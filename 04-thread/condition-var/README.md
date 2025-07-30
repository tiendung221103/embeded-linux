# Lập Trình Luồng - Bài 3: Đồng bộ hóa với Condition Variables (Mô hình Producer - Consumer)

## 1. Tổng quan

Bài tập này giải quyết bài toán kinh điển **Nhà sản xuất - Người tiêu dùng** (Producer - Consumer). Mục tiêu là sử dụng **condition variables** và **mutex** để giúp một luồng chờ đợi một điều kiện cụ thể xảy ra một cách hiệu quả, tránh sử dụng phương pháp kiểm tra liên tục (busy-waiting).

---

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```

**Vai trò các file:**

- `main.c`: Chương trình chính triển khai mô hình Producer - Consumer sử dụng condition variables.
- `Makefile`: Tự động hóa biên dịch và dọn dẹp.
- `README.md`: Tài liệu mô tả bài tập và hướng dẫn chạy.

---

## 3. Mô tả chương trình

- Gồm **một luồng Producer** và **một luồng Consumer**.
- Sử dụng hai biến toàn cục:
  - `int data`: lưu dữ liệu được tạo.
  - `int data_ready`: cờ báo dữ liệu đã sẵn sàng.

### Producer:
- Thực hiện vòng lặp **10 lần**.
- Mỗi lần tạo một số ngẫu nhiên.
- Ghi dữ liệu vào biến `data`, đặt `data_ready = 1`.
- Gọi `pthread_cond_signal()` để **báo hiệu Consumer**.

### Consumer:
- Chờ đợi (`pthread_cond_wait()`) cho đến khi `data_ready` được bật.
- Sau khi nhận tín hiệu, đọc giá trị từ `data`, xử lý, rồi đặt lại `data_ready = 0`.

### Đồng bộ hóa:
- Sử dụng một biến `pthread_mutex_t` để bảo vệ các thao tác truy cập và cập nhật `data`/`data_ready`.
- Dùng `pthread_cond_t` để chờ đợi và đánh thức chính xác giữa hai luồng.

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
```bash
Producer produced data: 69
Consumer received data: 69
Producer produced data: 71
Consumer received data: 71
```

### 4.3. Dọn dẹp
```bash
make clean
```

---

## 5. Giải thích

**Tại sao dùng Condition Variables?**
- Giúp một luồng chờ hiệu quả khi điều kiện chưa sẵn sàng, tránh việc kiểm tra liên tục (busy-waiting).

- Khi `pthread_cond_wait()` được gọi, luồng sẽ giải phóng mutex và tạm dừng, chờ đến khi có tín hiệu từ `pthread_cond_signal()`.

**Vai trò của mutex trong mô hình này?**
- Đảm bảo rằng chỉ một luồng truy cập hoặc thay đổi `data` và `data_ready` tại một thời điểm.

- `pthread_cond_wait()` phải được gọi khi đang giữ mutex và sẽ tự động giải phóng trong lúc chờ.

---

## 6. Kết luận

Bài tập này minh họa cách sử dụng `pthread_mutex_t` và `pthread_cond_t` để đồng bộ hóa hai luồng hoạt động theo kiểu phối hợp. Đây là cách tiếp cận chuẩn và hiệu quả cho các bài toán liên quan đến chia sẻ tài nguyên và tín hiệu giữa các luồng.



