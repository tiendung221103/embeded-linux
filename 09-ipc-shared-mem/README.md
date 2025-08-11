# POSIX Shared Memory – Chia sẻ dữ liệu giữa các tiến trình

## 1. Tổng quan

Bài tập này hướng dẫn cách sử dụng **POSIX Shared Memory** để chia sẻ thông tin sản phẩm giữa hai tiến trình: **producer** (người sản xuất) và **consumer** (người tiêu thụ).

- `producer` tạo và ghi thông tin sản phẩm vào bộ nhớ chung.
- `consumer` truy cập và đọc dữ liệu đó để hiển thị ra màn hình.

Sử dụng các API của POSIX như:

- `shm_open()` – mở hoặc tạo một đối tượng bộ nhớ chia sẻ.
- `ftruncate()` – thiết lập kích thước bộ nhớ.
- `mmap()` – ánh xạ vùng nhớ vào không gian tiến trình.
- `shm_unlink()` – xóa đối tượng bộ nhớ chia sẻ (sau khi xong).

---

## 2. Cấu trúc thư mục

```bash
.
├── Makefile
├── posix_shm_consumer.c
├── posix_shm_producer.c
└── README.md
```


---

## 3. Mô tả bài toán

### 3.1. Thông tin sản phẩm cần chia sẻ

```c
typedef struct {
    int id;
    char name[64];
    float price;
} Product;
```

### 3.2. Producer – ghi dữ liệu
- Gọi `shm_open()` với cờ `O_CREAT | O_RDWR`.

- Gọi `ftruncate()` để set kích thước vùng nhớ bằng `sizeof(Product)`.

- Dùng `mmap()` để ánh xạ vùng nhớ.

- Gán thông tin sản phẩm vào vùng nhớ được ánh xạ.

- In thông báo thành công và thoát.

### 3.3. Consumer – đọc dữ liệu
- Gọi `shm_open()` với cờ `O_RDONLY`.

- Dùng `mmap()` để ánh xạ vùng nhớ (read-only).

- Truy xuất và in ra nội dung sản phẩm.

- Gọi `shm_unlink()` để xóa bộ nhớ chia sẻ khỏi hệ thống.

## 4. Cách biên dịch và chạy
## 4.1. Biên dịch

```bash
make
```

## 4.2. Chạy chương trình

Mở terminal và chạy `producer`:

```bash
./producer
```

Mở terminal khác và chạy `consumer`:

```bash
./consumer
```

Kết quả mẫu:
```bash
Producer output:
Đã ghi sản phẩm: [ID: 101, Tên: Laptop, Giá: 1499.99]

Consumer output:
Đã đọc sản phẩm: [ID: 101, Tên: Laptop, Giá: 1499.99]
```

## 5. Kết luận
POSIX Shared Memory là một cơ chế IPC hiệu quả, giúp các tiến trình chia sẻ dữ liệu lớn với tốc độ cao hơn nhiều so với các cơ chế khác (pipe, socket...). Đây là nền tảng cho các hệ thống real-time, ứng dụng đa tiến trình, hoặc bộ nhớ chia sẻ liên tiến trình.