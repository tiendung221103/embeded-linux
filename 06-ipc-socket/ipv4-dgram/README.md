# IPC Socket - Bài 1.2: Gửi nhận dữ liệu bằng IPv4 Datagram Socket (UDP)

## 1. Tổng quan

Bài tập này giúp tìm hiểu cách sử dụng **IPv4 Datagram Socket** sử dụng giao thức **UDP** – một giao thức không kết nối (connectionless), nhanh, nhưng không đảm bảo thứ tự và độ tin cậy của dữ liệu. Mục tiêu là xây dựng chương trình **Client-Server** đơn giản, trong đó client gửi một chuỗi tới server thông qua UDP socket.

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

- `server.c`: Server UDP tạo socket, lắng nghe và nhận dữ liệu từ client.
- `client.c`: Client UDP gửi một chuỗi dữ liệu đến server.
- `Makefile`: Biên dịch chương trình server và client.
- `README.md`: Tài liệu mô tả và hướng dẫn chạy chương trình.

---

## 3. Mô tả chương trình

- **Server UDP:**
  - Tạo socket `AF_INET`, kiểu `SOCK_DGRAM`.
  - Gán với địa chỉ `127.0.0.1` và cổng (vd: `8888`) bằng `bind()`.
  - Sử dụng `recvfrom()` để nhận dữ liệu và in ra màn hình.

- **Client UDP:**
  - Tạo socket `AF_INET`, kiểu `SOCK_DGRAM`.
  - Gửi một chuỗi (vd: `"Hello via UDP!"`) tới địa chỉ `127.0.0.1:8888` bằng `sendto()`.

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
Server sẽ lắng nghe kết nối từ client.

Chạy client (mở Terminal khác):
```bash
./client
```

Kết quả mẫu:
- Server:
```bash
UDP Server is waiting for data on port 8080...
Server received: abcd
```

- Client:
```bash
Enter a message to send: abcd
Client sent: abcd
```

### 4.3. Dọn dẹp

```bash
make clean
```

## 5. Giải thích

Khác biệt so với TCP:
- UDP không thiết lập kết nối (no handshake).

- Gửi và nhận dữ liệu bằng `sendto()` và `recvfrom()` (thay vì `send()`/`recv()` trong TCP).

- Nhanh, nhưng không đảm bảo dữ liệu đến nơi, không thứ tự, không kiểm tra lỗi.

Ứng dụng của UDP:
- Truyền video/audio streaming.

- DNS, DHCP, VoIP, game thời gian thực.

## 6. Kết luận

Bài tập này giúp làm quen với cách truyền dữ liệu không kết nối bằng UDP socket. Đây là bước quan trọng để hiểu sự khác biệt về thiết kế giữa TCP và UDP khi lựa chọn mô hình giao tiếp IPC phù hợp với yêu cầu hiệu suất hoặc độ tin cậy.