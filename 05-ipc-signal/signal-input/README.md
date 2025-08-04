# IPC Signal - Bài 4: Chương trình chờ tín hiệu kết hợp chờ người dùng nhập liệu

## 1. Tổng quan

Bài tập này yêu cầu xây dựng chương trình có khả năng **xử lý tín hiệu hệ thống (SIGINT, SIGTERM)** và **đồng thời chờ người dùng nhập dữ liệu từ bàn phím** mà không bị gián đoạn. Đây là một kỹ thuật quan trọng trong lập trình hệ thống và giao diện dòng lệnh tương tác.

---

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```


**Vai trò các file:**

- `main.c`: Chương trình xử lý tín hiệu đồng thời chờ người dùng nhập dữ liệu bằng `select()` hoặc `poll()`.
- `Makefile`: Biên dịch và dọn dẹp nhanh.
- `README.md`: Mô tả và hướng dẫn sử dụng chương trình.

---

## 3. Mô tả chương trình

Chương trình thực hiện:

1. **Đăng ký các tín hiệu:**
   - `SIGINT`: khi nhận được, in ra dòng `"SIGINT received."`.
   - `SIGTERM`: thoát chương trình.

2. **Chờ dữ liệu người dùng nhập từ bàn phím:**
   - Sử dụng `select()` hoặc `poll()` để theo dõi file descriptor `stdin` mà không bị gián đoạn bởi tín hiệu.

3. **Xử lý dữ liệu nhập vào:**
   - Khi người dùng nhấn `Enter`, nội dung sẽ được đọc và in ra màn hình.

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

Kết quả mẫu:

- Khi nhập văn bản và nhấn Enter, chương trình in lại nội dung:
```bash
User input: abc
```
- Khi nhấn Ctrl + C (gửi SIGINT):
```bash
^CSIGINT received.
```
- Khi gửi tín hiệu SIGTERM:
```bash
kill -SIGTERM <PID>
```
--> Chương trình sẽ kết thúc

### 4.3. Dọn dẹp

```bash
make clean
```

## 5. Giải thích kỹ thuật

Vì sao dùng `select()` hoặc `poll()`?
- Cho phép chương trình đợi sự kiện từ stdin (bàn phím) mà không bị chặn vĩnh viễn.

- Khi có tín hiệu đến trong lúc đang chờ nhập liệu, chương trình vẫn xử lý được mà không cần vòng lặp bận (busy-waiting).

Xử lý tín hiệu đồng thời nhập liệu
- Nếu không dùng `select()` đúng cách, khi người dùng nhấn Ctrl+C, `scanf()` hoặc `fgets()` có thể bị gián đoạn hoặc không hoạt động như mong đợi.

- Kỹ thuật này thường được áp dụng trong ứng dụng dòng lệnh tương tác như `bash`, `top`, `htop`,...


## 6. Kết luận

Bài tập giúp bạn hiểu cách kết hợp xử lý tín hiệu và tương tác người dùng một cách mượt mà. Đây là kỹ thuật nâng cao nhưng cần thiết trong các ứng dụng CLI, shell và hệ thống phản hồi tín hiệu thời gian thực.