# Thao Tác Với Tiến Trình - Bài 1: Khởi tạo và Thu dọn Tiến trình

## 1. Tổng quan

Bài tập này khảo sát vòng đời cơ bản của một tiến trình trong hệ điều hành: **được tạo ra**, **thực thi**, **kết thúc** và **được tiến trình cha chờ đợi**.

Chương trình thực hiện việc tạo tiến trình con bằng `fork()`, sau đó tiến trình con sẽ thoát với một mã cụ thể, và tiến trình cha sẽ chờ và ghi nhận mã thoát của con.

---

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```
**Vai trò các file:**

- `main.c`: Chứa chương trình C thực hiện tạo tiến trình con, xử lý tiến trình cha và con.
- `Makefile`: Tự động hóa quá trình biên dịch và dọn dẹp.
- `README.md`: Tài liệu mô tả bài tập, cách chạy và giải thích.


---

## 3. Mô tả chương trình

- Tiến trình cha sử dụng `fork()` để tạo tiến trình con.
- Tiến trình con: 
  - In ra PID của chính nó.
  - Gọi `exit(10)` để thoát với mã thoát cụ thể.
- Tiến trình cha: 
  - In ra PID của chính nó và PID của tiến trình con.
  - Gọi `wait()` để chờ tiến trình con kết thúc.
  - Sử dụng `WIFEXITED()` và `WEXITSTATUS()` để kiểm tra trạng thái thoát và in ra mã thoát của tiến trình con.

---


## 4. Cách biên dịch và chạy

### 4.1. Biên dịch

Mở terminal và chạy lệnh:

```bash
make
```

### 4.2. Chạy chương trình

Sau khi biên dịch, chạy chương trình bằng lệnh:
```bash
./app
```

Kết quả mẫu:
``` bash
Parent process: My PID is 11377
Parent process: Child PID is 11378
Child process: My PID is 11378
Parent process: Child exited with status 10
```

### 4.3. Dọn dẹp

Xóa các file thực thi bằng lệnh:

```bash
make clean
```

### 5. Kết luận

Bài tập giúp hiểu rõ hơn về việc tạo tiến trình con trong C với fork(), cách cha và con tương tác thông qua wait() và cách xử lý mã thoát. Đây là nền tảng quan trọng trong lập trình hệ điều hành và quản lý tiến trình.

Các hàm cần nắm được cách sử dụng:
- fork(): tạo tiến trình con (nếu >0 là cha, =0 là con, <0 lỗi>)
- getpid(): để in ra process id của tiến trình
- wait(): đợi tiến trình con kết thúc
