# IPC Socket - Bài 1.3: Gửi nhận dữ liệu bằng Unix Stream Socket (Local TCP)

## 1. Tổng quan

Bài tập này giúp bạn làm quen với **Unix Domain Socket** ở chế độ **Stream** – một dạng giao tiếp liên tiến trình (IPC) trên cùng một máy tính, hoạt động giống như TCP nhưng không cần qua mạng. Đây là cách truyền dữ liệu **rất nhanh** và **an toàn** giữa các tiến trình cục bộ.

---

## 2. Cấu trúc thư mục

```bash
.
├── client.c
├── Makefile
├── README.md
└── server.c
```


**Vai trò các file:**

- `server.c`: Tạo Unix Stream socket, chờ client kết nối và nhận chuỗi dữ liệu.
- `client.c`: Tạo Unix Stream socket, kết nối tới server và gửi chuỗi dữ liệu.
- `Makefile`: Hỗ trợ biên dịch nhanh các file.
- `README.md`: Tài liệu mô tả bài tập.

---

## 3. Mô tả chương trình

- **Đường dẫn socket:** Cả client và server sử dụng cùng một file socket path, ví dụ: `/tmp/mysocket`.

### Server:
- Tạo socket kiểu `AF_UNIX`, `SOCK_STREAM`.
- Gắn với file `/tmp/mysocket` qua `bind()`.
- Lắng nghe kết nối bằng `listen()`.
- Chấp nhận client bằng `accept()`.
- Nhận và in ra chuỗi dữ liệu gửi đến.
- Xóa file socket (`unlink()`) sau khi kết thúc.

### Client:
- Tạo socket `AF_UNIX`, `SOCK_STREAM`.
- Kết nối tới `/tmp/mysocket` bằng `connect()`.
- Gửi chuỗi (ví dụ: `"Hello from local client!"`).

---

## 4. Cách biên dịch và chạy

### 4.1. Biên dịch

```bash
make
```
### 4.2. Chạy chương trình

Chạy server:
```bash
./server
```
Chạy client (mở Terminal khác):
```bash
./client
```

Kết quả mẫu:
- Server:
```bash
Unix Stream Server đang chờ kết nối tại /tmp/mysocket_stream...
Server nhận: Hello from client!
```

- Client:
```bash
Client đã gửi: Hello from client!
```

### 4.3. Dọn dẹp

```bash
make clean
```

## 5. Giải thích

Unix Stream Socket là gì?
- Là socket nội bộ (AF_UNIX), hoạt động như TCP nhưng không cần địa chỉ IP hay cổng.

- Truyền dữ liệu qua file hệ thống (ví dụ: /tmp/mysocket).

- Tốc độ nhanh và hiệu quả hơn TCP vì không qua giao thức mạng.

So sánh với IPv4 TCP:

| Đặc điểm  | IPv4 Stream Socket | Unix Stream Socket  |
| --------- | ------------------ | ------------------- |
| Phạm vi   | Qua mạng           | Cục bộ (nội bộ máy) |
| Giao thức | TCP (IP)           | File system         |
| Tốc độ    | Trung bình         | Rất nhanh           |
| Bảo mật   | Có thể bị nghe lén | Cục bộ, an toàn hơn |

## 6. Kết luận

Bài tập này minh họa cách sử dụng Unix Stream Socket để giao tiếp hiệu quả giữa các tiến trình trong cùng hệ thống. Đây là kỹ thuật IPC phổ biến trong nhiều ứng dụng hệ thống và phần mềm máy chủ cục bộ.