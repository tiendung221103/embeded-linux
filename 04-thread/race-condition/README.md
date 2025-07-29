# Các kiến thức cần nắm được:
- Khai báo và khởi tạo mutex với pthread_mutex_t và pthread_mutex_init().
- Sử dụng pthread_mutex_lock() để khóa mutex trước khi truy cập tài nguyên chia sẻ (ví dụ: counter).
- Sử dụng pthread_mutex_unlock() để mở khóa mutex sau khi hoàn thành.

# Vấn đề "Race Condition" và Giải pháp Mutex

Bài tập này minh họa vấn đề **race condition** – một lỗi kinh điển trong lập trình đa luồng – và giới thiệu **Mutex (Mutual Exclusion)** như một giải pháp cơ bản để đảm bảo sự an toàn khi truy cập tài nguyên dùng chung.

## 📝 Yêu cầu

1. Khai báo một biến toàn cục:
   long long counter = 0;
2. Tạo ra ba luồng. Mỗi luồng thực hiện một vòng lặp để tăng giá trị của counter lên 1,000,000 lần.

3. Sử dụng pthread_mutex_t để bảo vệ thao tác tăng counter:

    - Mỗi luồng phải khóa (lock) mutex trước khi tăng counter.

    - Sau khi tăng xong thì mở khóa (unlock) ngay.

4. Luồng chính sử dụng pthread_join() để chờ tất cả luồng hoàn thành, sau đó in ra giá trị cuối cùng của counter.
