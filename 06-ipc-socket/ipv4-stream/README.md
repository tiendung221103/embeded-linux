# IPC Socket - Bài 1.1: Gửi nhận dữ liệu bằng IPv4 Stream Socket (TCP)

## 1. Tổng quan

Bài tập này giúp làm quen với cách sử dụng **IPv4 Stream Socket** (giao thức TCP) để giao tiếp giữa tiến trình **Client-Server**. Đây là hình thức socket phổ biến nhất trong các ứng dụng mạng, bảo đảm truyền dữ liệu **theo thứ tự**, **đầy đủ**, và **đáng tin cậy**.

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

- `server.c`: Chương trình TCP server lắng nghe kết nối và nhận dữ liệu từ client.
- `client.c`: Chương trình TCP client kết nối tới server và gửi một chuỗi dữ liệu.
- `Makefile`: Hỗ trợ biên dịch cả server và client.
- `README.md`: Hướng dẫn sử dụng và giải thích chi tiết bài tập.

---

## 3. Mô tả chương trình

- **Server:**
  - Tạo socket `AF_INET` với `SOCK_STREAM`.
  - Gán socket với địa chỉ IP `127.0.0.1` và một cổng cụ thể (vd: `8080`).
  - Lắng nghe (`listen()`) và chấp nhận kết nối (`accept()`).
  - Nhận và in ra chuỗi dữ liệu từ client.

- **Client:**
  - Tạo socket `AF_INET` với `SOCK_STREAM`.
  - Kết nối tới `127.0.0.1:8080`.
  - Gửi một chuỗi dữ liệu, ví dụ: `"Hello from client!\n"`.

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
Server sẽ lắng nghe kết nối từ client.

Chạy client (mở Terminal khác):
```bash
./client
```

Kết quả mẫu:
- Server:
```bash
Server đang lắng nghe tại 127.0.0.1:8080...
Server nhận: abcd
```

- Client:
```bash
Nhập chuỗi cần gửi: abcd
Client đã gửi: abcd
```

### 4.3. Dọn dẹp

```bash
make clean
```

## 5. Giải thích

IPv4 Stream Socket dùng giao thức TCP, đảm bảo:

- Dữ liệu được truyền đúng thứ tự.

- Không bị mất mát.

- Kết nối hai chiều có trạng thái (stateful).

`bind()`, `listen()`, `accept()` là ba bước cơ bản để thiết lập server.

`connect()` được dùng ở phía client để tạo kết nối với server.

## 6. Kết luận

Bài tập này cung cấp kiến thức nền tảng về việc sử dụng socket TCP (IPv4) trong lập trình Client-Server. Đây là bước đầu tiên để xây dựng các ứng dụng mạng đáng tin cậy như web server, chat app, REST API,...