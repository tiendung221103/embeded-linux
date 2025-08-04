# IPC Signal - Bài 2: Sử dụng Tín hiệu SIGALRM để Tạo Timer

## 1. Tổng quan

Bài tập này hướng dẫn cách sử dụng tín hiệu **SIGALRM** và hàm `alarm()` để tạo một bộ đếm thời gian đơn giản. Khi tín hiệu được gửi tới sau mỗi giây, chương trình sẽ tăng biến đếm và in ra thời gian đã trôi qua.

---

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```


**Vai trò các file:**

- `main.c`: Chương trình sử dụng `alarm()` và `SIGALRM` để tạo timer đếm 10 giây.
- `Makefile`: Tự động hóa quá trình biên dịch và dọn dẹp.
- `README.md`: Hướng dẫn thực hiện bài tập và giải thích kỹ thuật.

---

## 3. Mô tả chương trình

- Sử dụng `alarm(1)` để gửi tín hiệu SIGALRM **mỗi giây**.
- Trong hàm xử lý tín hiệu `SIGALRM`:
  - Tăng biến đếm.
  - In ra dòng `"Timer: <giây> seconds"`.
  - Gọi lại `alarm(1)` để hẹn lần tiếp theo.
- Dừng chương trình khi đếm đủ 10 giây.

### Gợi ý:

- Dùng biến toàn cục `volatile sig_atomic_t counter = 0;`.
- Dùng `signal(SIGALRM, handler)` để đăng ký xử lý tín hiệu.
- Trong `main()`, sử dụng `pause()` hoặc vòng lặp chờ đợi đến khi đủ số giây.

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
Timer: 1 seconds
Timer: 2 seconds
...
Timer: 10 seconds
Done.
```

### 4.3. Dọn dẹp

```bash
make clean
```


## 5. Giải thích kỹ thuật

Vai trò của `alarm()` và `SIGALRM`
- `alarm(n)` sẽ gửi tín hiệu `SIGALRM` cho tiến trình sau n giây.

- `signal(SIGALRM, handler)` gắn một hàm xử lý khi nhận tín hiệu này.

Điều gì xảy ra nếu không gọi lại `alarm(1)` trong hàm xử lý?
- Tín hiệu chỉ được gửi một lần duy nhất.

- Chương trình không đếm tiếp và dừng ở lần đầu tiên.

Sự lặp lại của `alarm(1)` trong handler
- Gọi lại `alarm(1)` giúp chương trình lập lại tín hiệu mỗi giây một cách thủ công.

- Đây là cách tạo "đồng hồ đếm giây" đơn giản mà không dùng vòng lặp chính.

## 6. Kết luận

Bài tập này cung cấp kiến thức thực tế về cách sử dụng tín hiệu thời gian trong hệ điều hành, phục vụ cho việc lập lịch và đồng hồ trong các chương trình chạy nền hoặc hệ thống nhúng.