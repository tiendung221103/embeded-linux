# Hệ thống Xử lý Tác vụ Ưu tiên bằng POSIX Message Queue

## 1. Tổng quan

Ứng dụng này mô phỏng một hệ thống xử lý tác vụ trong đó **các tác vụ được thực thi theo độ ưu tiên**. Sử dụng **POSIX Message Queue**, hai tiến trình riêng biệt sẽ phối hợp:

- **`task_dispatcher`**: Gửi tác vụ với độ ưu tiên cụ thể.
- **`task_worker`**: Nhận và xử lý tác vụ quan trọng nhất trước.

Điểm nổi bật của POSIX Queue là hỗ trợ **độ ưu tiên nội tại**: tác vụ có độ ưu tiên cao hơn sẽ luôn được xử lý trước, **bất kể thời điểm được gửi**.

---

## 2. Cấu trúc thư mục

```bash
.
├── Makefile
├── README.md
├── task_dispatcher.c
└── task_worker.c
```

---

## 3. Thành phần và chức năng

### 3.1. `task_worker` – Tiến trình xử lý

- Khởi tạo hàng đợi POSIX (hoặc mở nếu đã tồn tại).
- Vòng lặp vô tận:
  - Nhận tin nhắn từ queue bằng `mq_receive()`.
  - Luôn nhận tác vụ **có độ ưu tiên cao nhất trước**.
  - In ra nội dung và độ ưu tiên.
  - Mô phỏng xử lý bằng `sleep(1)`.
- Bắt tín hiệu `SIGINT` (`Ctrl+C`) để:
  - Đóng hàng đợi (`mq_close()`).
  - Xóa hàng đợi khỏi hệ thống (`mq_unlink()`).

### 3.2. `task_dispatcher` – Tiến trình gửi

- Nhận 2 tham số dòng lệnh:
  - `priority`: độ ưu tiên (số nguyên).
  - `task_content`: nội dung tác vụ (chuỗi).
- Mở queue đã tồn tại bằng `mq_open()` với cờ `O_WRONLY`.
- Gửi tác vụ vào queue bằng `mq_send()` kèm độ ưu tiên.
- In xác nhận và thoát.

---

## 4. Biên dịch chương trình

POSIX Queue cần liên kết với thư viện real-time (`-lrt`):

```bash
gcc task_worker.c -o task_worker -lrt
gcc task_dispatcher.c -o task_dispatcher -lrt
```

## 5. Cách chạy và kiểm tra

Bước 1: Khởi chạy tiến trình `worker`
```bash
./task_worker
```

Bước 2: Gửi các tác vụ từ terminal khác
```bash
./task_dispatcher 10 "Update weekly report"
./task_dispatcher 30 "CRITICAL: Restart web server"
./task_dispatcher 20 "Backup user database"
```

Quan sát đầu ra tại `task_worker`
Dù các task được gửi không theo thứ tự, chương trình vẫn xử lý theo độ ưu tiên giảm dần:
```bash
Processing task (Priority: 30): CRITICAL: Restart web server
Processing task (Priority: 20): Backup user database
Processing task (Priority: 10): Update weekly report
```

**Khi task_worker nhận Ctrl+C, chương trình:**

Không thoát ngay, mà gọi:

- `mq_close()` để đóng queue.

- `mq_unlink()` để xoá queue khỏi hệ thống.

Việc này đảm bảo không để lại queue "mồ côi" trong `/dev/mqueue`.

## 6. Kết luận

Hệ thống này giúp bạn hiểu:

- Cách thiết lập và sử dụng POSIX Message Queue.

- Cơ chế ưu tiên tác vụ tự động khi truyền message.

- Giao tiếp giữa nhiều tiến trình độc lập một cách hiệu quả và phi đồng bộ
