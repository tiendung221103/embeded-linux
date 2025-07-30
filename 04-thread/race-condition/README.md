# Lập Trình Luồng - Bài 2: Vấn đề "Race Condition" và Giải pháp Mutex

## 1. Tổng quan

Bài tập này minh họa một trong những vấn đề phổ biến và nguy hiểm trong lập trình đa luồng: **race condition** – khi nhiều luồng truy cập và sửa đổi tài nguyên dùng chung mà không được đồng bộ đúng cách. Bài tập cũng giới thiệu cách sử dụng **Mutex (Mutual Exclusion)** để đảm bảo an toàn khi thao tác với tài nguyên chia sẻ.

---

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```

**Vai trò các file:**

- `main.c`: Chứa chương trình minh họa tăng biến đếm `counter` dùng nhiều luồng, có sử dụng mutex.
- `Makefile`: Biên dịch và dọn dẹp tự động.
- `README.md`: Tài liệu hướng dẫn bài tập.

---

## 3. Mô tả chương trình

- Khai báo một biến toàn cục `long long counter = 0;`
- Tạo **ba luồng**, mỗi luồng thực hiện một vòng lặp tăng biến `counter` lên **1,000,000 lần**.
- Mỗi thao tác tăng `counter` phải được bảo vệ bằng **pthread_mutex_t**:
  - Gọi `pthread_mutex_lock()` trước khi tăng.
  - Gọi `pthread_mutex_unlock()` sau khi tăng.

- Luồng chính sử dụng `pthread_join()` để chờ **cả ba luồng hoàn thành**, sau đó in ra giá trị cuối cùng của `counter`.

---

## 4. Cách biên dịch và chạy

### 4.1. Biên dịch

```bash
make
```

### 4.2. Chạy chương trình
 
 ```bash
 ./app
 ```

 Kết quả mẫu(có dùng Mutex):

 ```bash
 Expected counter value: 3000000
Actual counter value  : 3000000
```

Nếu bỏ mutex, kết quả có thể sai lệch như:

```bash
Expected counter value: 3000000
Actual counter value  : 1402824
```

## 4.3. Dọn dẹp

```bash
make clean
```
---

## 5. Giải thích

Tại sao cần mutex?
- Trong môi trường đa luồng, khi nhiều luồng đồng thời đọc và ghi vào biến toàn cục, kết quả có thể không chính xác do tranh chấp tài nguyên.
- Mutex đảm bảo rằng chỉ một luồng có quyền truy cập tài nguyên tại một thời điểm.


Điều gì xảy ra nếu không dùng mutex?
- Các thao tác như counter++ không nguyên tử — gồm nhiều bước (đọc, tăng, ghi lại).
- Khi hai luồng cùng tăng, có thể xảy ra ghi đè, dẫn đến mất thao tác tăng.
- Kết quả cuối cùng của biến counter sẽ sai lệch và không ổn định qua mỗi lần chạy.

---

## 6. Kết luận

Bài tập cung cấp cái nhìn rõ ràng về nguy cơ race condition và cách sử dụng mutex như một cơ chế đồng bộ hóa truy cập tài nguyên dùng chung. Đây là một trong những kiến thức thiết yếu khi lập trình đa luồng an toàn.








