# IPC Signal - Bài 3: Tạo Giao tiếp giữa Các Process bằng Tín hiệu

## 1. Tổng quan

Bài tập này giới thiệu cách sử dụng **tín hiệu (signal)** để giao tiếp giữa **tiến trình cha và tiến trình con**. Cụ thể, chương trình sẽ sử dụng tín hiệu **SIGUSR1** để truyền thông tin từ tiến trình cha sang tiến trình con định kỳ.

---

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```


**Vai trò các file:**

- `main.c`: Chương trình chính sử dụng `fork()` để tạo process con và giao tiếp bằng tín hiệu SIGUSR1.
- `Makefile`: Hỗ trợ biên dịch nhanh và dọn dẹp.
- `README.md`: Hướng dẫn chi tiết bài tập.

---

## 3. Mô tả chương trình

- Chương trình tạo **tiến trình con** bằng `fork()`.
- **Tiến trình cha**:
  - Gửi tín hiệu `SIGUSR1` tới tiến trình con **mỗi 2 giây**.
  - Gửi tổng cộng **5 lần**, sau đó kết thúc.

- **Tiến trình con**:
  - Đăng ký hàm xử lý `SIGUSR1` bằng `signal(SIGUSR1, handler)`.
  - Mỗi lần nhận tín hiệu, in ra thông báo:

    ```
    Received signal from parent
    ```

- Sau khi nhận 5 tín hiệu, tiến trình con cũng kết thúc.

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
Child: Received signal from parent (1)
Child: Received signal from parent (2)
Child: Received signal from parent (3)
Child: Received signal from parent (4)
Child: Received signal from parent (5)
Child: Received 5 signals. Exiting.
Parent: Child exited. Program done.
```

### 4.3. Dọn dẹp

```bash
make clean
```

## 5. Giải thích kỹ thuật

Tại sao dùng `kill()`?
- Hàm `kill(pid, signal)` cho phép một tiến trình gửi tín hiệu đến một tiến trình khác (trong trường hợp này là từ cha đến con).

- Đây là cách phổ biến để tiến trình cha chủ động thông báo điều gì đó cho con (hoặc ngược lại).

Tại sao dùng `signal()` trong con?
- Tiến trình con cần đăng ký một hàm xử lý tín hiệu để biết khi nào tín hiệu từ cha được gửi đến.

- Nếu không đăng ký, tiến trình sẽ dùng hành vi mặc định (thường là bị kết thúc).


## 6. Kết luận

Bài tập minh họa cách giao tiếp giữa các tiến trình bằng tín hiệu. Đây là cơ chế IPC (Inter-Process Communication) đơn giản nhưng hữu ích để đồng bộ hóa hoặc điều khiển hành vi giữa các process.