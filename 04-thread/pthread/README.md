# Lập Trình Luồng - Bài 1: Tạo và Chạy Luồng Cơ bản

## 1. Tổng quan

Bài tập này giúp làm quen với khái niệm và thao tác cơ bản khi làm việc với **luồng (thread)** trong lập trình C sử dụng thư viện `pthread.h`. Mục tiêu là tạo nhiều luồng song song, cho chúng thực thi và đảm bảo luồng chính chờ luồng con hoàn thành.

---

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```

**Vai trò các file:**

- `main.c`: Chứa chương trình C tạo và quản lý hai luồng.
- `Makefile`: Hỗ trợ biên dịch và dọn dẹp nhanh.
- `README.md`: Hướng dẫn sử dụng và giải thích bài tập.

---

## 3. Mô tả chương trình

- Tạo **hai luồng mới** bằng cách sử dụng hàm `pthread_create()`.
- Mỗi luồng, khi bắt đầu thực thi, sẽ **in ra một chuỗi thông báo chứa ID** của luồng, ví dụ:
```bash
Thread với ID 140700016543488 đang chạy!
```
- Luồng chính sử dụng `pthread_join()` để chờ **cả hai luồng con kết thúc hoàn toàn** trước khi chương trình chính kết thúc.

---

## 4. Cách biên dịch và chạy

### 4.1. Biên dịch chương trình

```bash
make
```

### 4.2. Chạy chương trình

```bash
./app
```

Kết quả mẫu:
```bash
Thread with ID 0 is running!
Thread with ID 1 is running!
Both threads have finished. Program completed.
```
(Lưu ý: ID luồng thay đổi mỗi lần chạy)

### 4.3. Dọn dẹp

```bash
make clean
```
---

## 5. Giải thích 

**pthread_create()**
- Dùng để tạo một luồng mới.

- Cú pháp:

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);
```

Tham số chính:
- thread: con trỏ tới biến chứa ID của luồng được tạo.
- attr: thuộc tính của luồng (có thể NULL để mặc định).
- start_routine: hàm mà luồng sẽ chạy.
- arg: đối số truyền vào hàm luồng.

**pthread_join()**
- Dùng để đồng bộ: chờ luồng được chỉ định kết thúc.

- Cú pháp:

```c
int pthread_join(pthread_t thread, void **retval);
```

- Nếu không gọi pthread_join(), luồng chính có thể kết thúc trước khi luồng con hoàn thành.

**Một luồng kết thúc khi nào?**
- Khi hàm luồng kết thúc (return từ start_routine()).

- Hoặc khi gọi pthread_exit() từ trong luồng.

- Lúc đó, luồng giải phóng tài nguyên và kết thúc sạch sẽ.

---

## 6. Kết luận

Bài tập cung cấp nền tảng để hiểu cách tạo và quản lý luồng trong lập trình đa luồng. Việc sử dụng pthread_create() và pthread_join() giúp đảm bảo chương trình hoạt động đồng bộ và an toàn với nhiều luồng song song.
