# Các kiến thức cần nắm được:
- Khai báo và khởi tạo pthread_cond_t với pthread_cond_init()
- Sử dụng pthread_cond_wait() để luồng Consumer chờ tín hiệu từ Producer.
- Sử dụng pthread_cond_signal() để Producer gửi tín hiệu khi dữ liệu sẵn sàng.



# Đồng bộ hóa với Condition Variables (Mô hình Producer - Consumer)

Bài tập này giải quyết bài toán **“Nhà sản xuất - Người tiêu dùng” (Producer - Consumer)** kinh điển.  
Mục tiêu là sử dụng **Condition Variables** để cho phép luồng Consumer chờ đợi một cách hiệu quả khi chưa có dữ liệu mới từ Producer, tránh việc kiểm tra liên tục gây tốn CPU (busy-waiting).

## 📝 Yêu cầu

1. Xây dựng chương trình với **một luồng Producer** và **một luồng Consumer**.
2. Sử dụng **biến toàn cục `data`** và **biến cờ `data_ready`** để chia sẻ dữ liệu giữa hai luồng.
3. **Producer**:
   - Lặp 10 lần.
   - Mỗi lần tạo một số nguyên ngẫu nhiên.
   - Ghi vào `data`, bật cờ `data_ready = 1`, sau đó gọi `pthread_cond_signal()` để **báo hiệu Consumer**.

4. **Consumer**:
   - Dùng `pthread_cond_wait()` để **chờ Producer báo hiệu**.
   - Khi được đánh thức, đọc giá trị từ `data`, in ra kết quả, và đặt lại `data_ready = 0`.

5. Sử dụng một **Mutex** và một **Condition Variable (`pthread_cond_t`)** để **đồng bộ hóa** chính xác hai luồng.
