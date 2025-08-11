# 📌 Bài toán Multiple Producer – Multiple Consumer với POSIX Shared Memory & Semaphores

## 1. Mô tả bài toán
Bài tập mô phỏng hoạt động của một **nhà hàng** với:
- **Đầu bếp (Producer)** liên tục nấu món ăn và đặt lên **mâm thức ăn** (bounded buffer).
- **Khách hàng (Consumer)** lấy món ăn từ các mâm này theo nhu cầu.
- Giao tiếp **liên tiến trình (IPC)** thông qua **POSIX Shared Memory** (`mmap()`).
- Đồng bộ hóa bằng **POSIX Semaphores** để tránh xung đột.

---

## 2. Thành phần chương trình

### 2.1. Tiến trình con
Tổng cộng **5 tiến trình con**:
- **2 Đầu bếp (Producers)**:
  - Chef Donatello (món **không chay**):  
    1. Fettuccine Chicken Alfredo  
    2. Garlic Sirloin Steak  
  - Chef Portecelli (món **chay**):  
    1. Pistachio Pesto Pasta  
    2. Avocado Fruit Salad  
  - Hành vi:
    - Nấu ngẫu nhiên 1 trong 2 món đặc trưng.
    - Đặt món lên mâm tương ứng (tốc độ ngẫu nhiên 1–5 giây).
    - Nếu mâm đầy → phải **chờ**.

- **3 Khách hàng (Consumers)**:
  - Khách 1: Muốn món **không chay**.
  - Khách 2: Muốn món **chay**.
  - Khách 3 (Hybrid): Muốn **1 món chay và 1 món không chay**.
  - Hành vi:
    - Nếu không có món phù hợp → **chờ**.
    - Sau khi lấy món → chờ 10–15 giây mới lấy tiếp.

---

### 2.2. Mâm thức ăn (Bounded Buffers)
- **2 mâm** trong shared memory:
  - Mâm chay  
  - Mâm không chay  
- Kích thước: **10 ô/mâm**.
- Mỗi ô chứa `int`:
  - `0` → trống
  - `1` → món chay
  - `2` → món không chay

---

### 2.3. Đồng bộ hóa
Mỗi mâm có **3 semaphore**:
- `mutex` (binary, khởi tạo = 1): Đảm bảo 1 tiến trình truy cập mâm tại một thời điểm.
- `empty` (counting, khởi tạo = 10): Số lượng ô trống còn lại.
- `full` (counting, khởi tạo = 0): Số lượng ô đã có món.

---

### 2.4. Logging
- **Đầu bếp/Khách hàng**: Mỗi lần thêm hoặc lấy món → in ra thông tin:
  ```
  Chef Donatello: Added Fettuccine Chicken Alfredo to Non-Vegan tray.
  Customer 2: Took Pistachio Pesto Pasta from Vegan tray.
  ```
- **Tiến trình cha**: Cứ 10 giây → in trạng thái 2 mâm:
  ```
  Items in Non-Vegan tray: 4/10
  Items in Vegan tray: 7/10
  ```

---

## 3. Cách chạy chương trình
### Biên dịch:
```bash
gcc main.c -o restaurant -lpthread -lrt
```

### Chạy:
```bash
./restaurant
```

---

## 4. Các hàm API chính

| Hàm            | Mô tả |
|----------------|-------|
| `fork()`       | Tạo tiến trình con. |
| `mmap()`       | Tạo vùng bộ nhớ chia sẻ giữa các tiến trình. |
| `sem_init()`   | Khởi tạo semaphore. |
| `sem_wait()`   | Giảm giá trị semaphore (chờ nếu giá trị = 0). |
| `sem_post()`   | Tăng giá trị semaphore, báo hiệu tài nguyên sẵn sàng. |
| `sleep()`      | Tạm dừng tiến trình một khoảng thời gian. |

---

## 5. Giải thích cơ chế hoạt động
- **Producer**:
  1. `sem_wait(empty)` → chờ nếu mâm đầy.
  2. `sem_wait(mutex)` → khóa mâm.
  3. Thêm món vào mâm.
  4. `sem_post(mutex)` → mở khóa.
  5. `sem_post(full)` → báo có món mới.

- **Consumer**:
  1. `sem_wait(full)` → chờ nếu mâm trống.
  2. `sem_wait(mutex)` → khóa mâm.
  3. Lấy món từ mâm.
  4. `sem_post(mutex)` → mở khóa.
  5. `sem_post(empty)` → báo có ô trống.

---

## 6. Mở rộng
- Có thể tăng số lượng đầu bếp và khách hàng.
- Thêm cơ chế ưu tiên loại khách hàng.
- Thêm tính năng ghi log ra file.
