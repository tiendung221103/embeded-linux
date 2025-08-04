# IPC Signal - Bài 1: Bắt và Xử lý Tín hiệu SIGINT

## 1. Tổng quan

Bài tập này giúp bạn làm quen với **tín hiệu (signal)** trong hệ điều hành – một cơ chế giao tiếp giữa hệ thống và tiến trình. Cụ thể, bài tập yêu cầu xử lý tín hiệu **SIGINT** (thường được gửi khi nhấn tổ hợp phím `Ctrl + C`).

---

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```


**Vai trò các file:**

- `main.c`: Chương trình bắt tín hiệu SIGINT, xử lý và theo dõi số lần nhận được.
- `Makefile`: Tự động biên dịch chương trình và dọn dẹp.
- `README.md`: Tài liệu mô tả bài tập và hướng dẫn sử dụng.

---

## 3. Mô tả chương trình

- Đăng ký hàm xử lý cho tín hiệu SIGINT bằng `signal(SIGINT, handler_func)`.
- Trong hàm xử lý:
  - In ra dòng `"SIGINT received"` mỗi khi nhận tín hiệu.
  - Tăng một biến đếm toàn cục.
- Chương trình tiếp tục chạy **cho đến khi nhận SIGINT lần thứ 3**, sau đó in thông báo kết thúc và thoát.

### Gợi ý:

- Dùng một biến toàn cục `volatile sig_atomic_t sigint_count = 0;` để theo dõi số lần nhận tín hiệu.
- Hàm `signal()` nên được đăng ký càng sớm càng tốt trong `main()`.

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

Trong lúc chương trình đang chạy, nhấn Ctrl + C nhiều lần để gửi tín hiệu SIGINT.

Kết quả mẫu:

```bash
^CSIGINT received (1)
^CSIGINT received (2)
^CSIGINT received (3)
Received SIGINT 3 times. Exiting.
```

### 4.3. Dọn dẹp

```bash
make clean
```

## 5. Giải thích kỹ thuật

Tín hiệu `SIGINT` là gì?
- Là tín hiệu ngắt (interrupt) gửi tới tiến trình đang chạy khi người dùng nhấn Ctrl + C trong terminal.

- Mặc định, nếu không xử lý, chương trình sẽ bị thoát ngay lập tức.

Điều gì xảy ra nếu không xử lý `SIGINT`?
- Hành vi mặc định của hệ điều hành là chấm dứt chương trình ngay lập tức.

- Việc xử lý tín hiệu cho phép bạn kiểm soát tiến trình tốt hơn – ví dụ: xác nhận thoát, ghi log, giải phóng tài nguyên,...

## 6. Kết luận

Bài tập cung cấp kiến thức thực tế về quản lý tín hiệu trong lập trình hệ thống. Việc xử lý tín hiệu đúng cách giúp chương trình hoạt động an toàn và thân thiện hơn với người dùng trong môi trường dòng lệnh.