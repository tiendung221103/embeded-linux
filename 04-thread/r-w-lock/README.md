# Tối ưu hóa Truy cập với Read-Write Lock

Bài tập này mô phỏng cơ chế đồng bộ hóa truy cập tài nguyên dữ liệu với **Read-Write Lock**, một giải pháp giúp nhiều luồng có thể đọc dữ liệu đồng thời, nhưng đảm bảo **độc quyền cho luồng ghi**.

## 📝 Yêu cầu

1. Viết chương trình mô phỏng một **tài nguyên dữ liệu** là một biến nguyên toàn cục.
2. Tạo ra **5 luồng đọc (Reader)** và **2 luồng ghi (Writer)**.
3. **Reader**:
   - Chỉ đọc dữ liệu.
   - In ra giá trị hiện tại của tài nguyên.

4. **Writer**:
   - Tăng giá trị tài nguyên lên 1 mỗi lần ghi.

5. Sử dụng **pthread_rwlock_t** để đồng bộ hóa:
   - Reader sử dụng `pthread_rwlock_rdlock()` để **yêu cầu khóa đọc**.
   - Writer sử dụng `pthread_rwlock_wrlock()` để **yêu cầu khóa ghi**.
   - Sau khi thao tác xong, cả hai đều phải gọi `pthread_rwlock_unlock()` để **giải phóng khóa**.

6. Quan sát kết quả in ra để thấy:
   - **Nhiều Reader** có thể chạy **song song**.
   - **Writer** luôn phải có **độc quyền truy cập**.


