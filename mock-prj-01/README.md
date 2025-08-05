# Ứng dụng Chat Peer-to-Peer (P2P) trên Giao diện Dòng lệnh (CLI)

## 1. Tổng quan

Ứng dụng này mô phỏng một hệ thống chat **P2P** (peer-to-peer), nơi mỗi người dùng vừa là client vừa là server. Không cần máy chủ trung tâm – các máy tính (peer) có thể trực tiếp gửi/nhận tin nhắn lẫn nhau thông qua các lệnh dòng lệnh.

Mỗi tiến trình sẽ:
- **Lắng nghe kết nối đến từ peer khác**.
- **Chủ động kết nối tới peer khác**.
- **Gửi, nhận, quản lý kết nối thông qua lệnh CLI**.

---

## 2. Cấu trúc thư mục

```bash
.
├── main.c 
├── README.md 
└── Makefile 
```

---

## 3. Chức năng đã triển khai

### 3.1. Lệnh cơ bản

| Lệnh                  | Mô tả                                                  |
|-----------------------|--------------------------------------------------------|
| `help`               | Hiển thị danh sách lệnh và hướng dẫn sử dụng           |
| `myip`               | In ra IP cục bộ của máy                                |
| `myport`             | In ra cổng đang lắng nghe kết nối                      |

### 3.2. Quản lý kết nối

| Lệnh                            | Mô tả                                                            |
|----------------------------------|------------------------------------------------------------------|
| `connect <ip> <port>`          | Tạo kết nối TCP tới peer khác                                    |
| `list`                         | Liệt kê danh sách các peer đang kết nối                          |
| `terminate <id>`              | Ngắt kết nối với peer theo ID trong danh sách                    |
| `exit`                         | Đóng tất cả kết nối và thoát chương trình                        |

### 3.3. Gửi/nhận tin nhắn

| Lệnh                             | Mô tả                                                          |
|----------------------------------|----------------------------------------------------------------|
| `send <id> <message>`           | Gửi một tin nhắn (tối đa 100 ký tự) tới peer với ID chỉ định  |

**Mỗi khi nhận được tin nhắn**, chương trình sẽ tự động in ra nội dung và địa chỉ của người gửi.

---

## 4. Hướng dẫn sử dụng

### 4.1. Biên dịch chương trình

```bash
make
```

### 4.2. Chạy chương trình

Mỗi peer cần chạy chương trình với một cổng:
- Trên máy A:
```bash
./chat_p2p 4000
```
- Trên máy B (mở một Terminal khác):
```bash
./chat_p2p 5000
```

- Tại máy A, kết nối tới máy B:
```bash
connect <ip_cua_may_B> 5000
```

### 5. Mô hình đa luồng

Chương trình sử dụng đa luồng (multi-threading) để xử lý đồng thời:

| Luồng            | Vai trò                                                |
| ---------------- | ------------------------------------------------------ |
| Luồng chính      | Nhận lệnh từ người dùng                                |
| Luồng lắng nghe  | `listener_thread`: chấp nhận kết nối mới               |
| Luồng xử lý peer | `peer_handler_thread`: nhận dữ liệu từ một peer cụ thể |


### 6. Cấu trúc dữ liệu chính
```c
typedef struct {
    int id;
    int sockfd;
    char ip[INET_ADDRSTRLEN];
    int port;
    pthread_t thread_id;
    int active;
} Peer;
```
Mỗi peer được lưu vào mảng peers[MAX_PEERS], và quản lý qua ID duy nhất.

### 7. Một số tính năng đã xử lý

Kiểm tra lỗi IP/port khi `connect`.

Không cho phép tự kết nối đến chính mình.

Không treo chương trình nếu peer ngắt kết nối đột ngột.

Xử lý kết thúc chương trình bằng exit và dọn dẹp socket, luồng.

### 8. Giải thích chi tiết các đoạn mã phức tạp

Dưới đây là một số đoạn code quan trọng trong chương trình kèm theo giải thích chi tiết:

🧩 1. Lấy địa chỉ IP cục bộ (myip)
```c
int tmp_sock = socket(AF_INET, SOCK_DGRAM, 0);
struct sockaddr_in temp_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(80)
};
inet_pton(AF_INET, "8.8.8.8", &temp_addr.sin_addr);
connect(tmp_sock, (struct sockaddr *)&temp_addr, sizeof(temp_addr));
```

Giải thích:
Chúng ta kết nối tạm tới 8.8.8.8 (Google DNS) để “ép” hệ thống chọn một IP cục bộ phù hợp. Mặc dù không gửi gói nào thực sự, connect() sẽ cấu hình socket, sau đó ta dùng getsockname() để lấy địa chỉ IP cục bộ mà hệ thống sẽ sử dụng nếu gửi gói ra ngoài. Cách này hữu ích khi máy có nhiều card mạng (multihomed).

🧩 2. Hàm `add_peer()`

```c
for (int i = 0; i < MAX_PEERS; i++) {
    if (!peers[i].active) {
        peers[i] = (Peer){ ... };
        ...
        return peers[i].id;
    }
}
return -1;
```
Giải thích:
Hàm tìm vị trí trống đầu tiên trong mảng peers[] (dựa vào cờ active == 0), sau đó khởi tạo thông tin kết nối mới vào đó. Nếu vượt quá MAX_PEERS, trả về -1. Đây là quản lý kết nối dạng tĩnh, phù hợp với hệ thống đơn giản.

🧩 3. Nhận tin nhắn trong luồng `peer_handler_thread`

```c
while (1) {
    int n = recv(sockfd, buffer, MAX_MSG_LEN, 0);
    if (n <= 0) {
        ...
        break;
    }
    printf("\n[Message received]: %s\n> ", buffer);
    fflush(stdout);
}
```
Giải thích:
Mỗi luồng sẽ lắng nghe dữ liệu từ một peer cụ thể.

- Nếu `recv()` trả về 0 hoặc âm => kết nối bị đóng.

- Tin nhắn nhận được được in ra màn hình ngay, và thêm dấu > để gợi ý nhập tiếp lệnh.


### 9. Kết luận

Ứng dụng này là một minh chứng cho mô hình chat P2P sử dụng socket TCP, xử lý đa luồng để duy trì nhiều kết nối đồng thời. Nó giúp người học hiểu rõ cách:

- Quản lý socket.

- Xử lý I/O đồng thời bằng luồng.

- Giao tiếp mạng không cần máy chủ trung tâm.