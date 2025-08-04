# IPC Socket - Bài 1.4: Gửi nhận dữ liệu bằng Unix Datagram Socket (Local UDP)

## 1. Tổng quan

Bài tập này giúp bạn thực hành với **Unix Domain Datagram Socket** – một dạng socket không kết nối (tương tự UDP), dùng để giao tiếp giữa các tiến trình trên cùng một máy. Khác với Unix Stream, kiểu datagram không đảm bảo thứ tự, cũng như không cần thiết lập kết nối trước khi truyền dữ liệu.

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

- `server.c`: Tạo socket `SOCK_DGRAM` và nhận dữ liệu từ client.
- `client.c`: Gửi một chuỗi dữ liệu đến địa chỉ socket của server.
- `Makefile`: Biên dịch chương trình.
- `README.md`: Tài liệu hướng dẫn bài tập.

---

## 3. Mô tả chương trình

- **Server socket path:** `/tmp/mysocket_dgram_server`
- **Client socket path:** `/tmp/mysocket_dgram_client`

### Server:
- Tạo socket `AF_UNIX`, kiểu `SOCK_DGRAM`.
- Gán socket với `/tmp/mysocket_dgram_server` bằng `bind()`.
- Nhận dữ liệu từ client bằng `recvfrom()`.
- In dữ liệu ra màn hình và kết thúc sau khi nhận xong.
- Gọi `unlink()` để xóa socket file sau khi kết thúc.

### Client:
- Tạo socket `AF_UNIX`, kiểu `SOCK_DGRAM`.
- Gán socket với `/tmp/mysocket_dgram_client` (cần thiết để nhận phản hồi hoặc để socket có thể gửi đi).
- Gửi một chuỗi (ví dụ `"Hello via Unix Datagram"`) tới `/tmp/mysocket_dgram_server` bằng `sendto()`.

---

## 4. Cách biên dịch và chạy

### 4.1. Biên dịch chương trình

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
Unix Datagram Server đang chờ dữ liệu tại /tmp/mysocket_dgram_server...
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

Unix Datagram Socket là gì?
- Là socket nội bộ (AF_UNIX + SOCK_DGRAM) dùng để gửi các gói tin giữa các tiến trình.

- Không cần connect(), không tạo luồng giao tiếp lâu dài.

- Mỗi lần gửi đều độc lập (giống UDP).

So sánh với Unix Stream Socket:

| Đặc điểm        | Stream Socket       | Datagram Socket     |
| --------------- | ------------------- | ------------------- |
| Kiểu giao tiếp  | Liên tục (TCP-like) | Gói tin (UDP-like)  |
| Cần `connect()` | Có                  | Không               |
| Độ tin cậy      | Cao                 | Thấp hơn            |
| Phù hợp cho     | Dữ liệu dài         | Gửi thông báo nhanh |

## 6. Kết luận

Unix Datagram Socket là công cụ IPC nhẹ, nhanh, không kết nối – phù hợp với các tình huống gửi thông báo hoặc giao tiếp nhanh giữa các tiến trình cục bộ. Bài tập này giúp bạn hiểu được cách dùng socket kiểu UDP trong không gian Unix nội bộ.