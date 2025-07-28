
Các họ hàm exec:
- execl()	Gọi chương trình mới với danh sách tham số rời rạc
- execlp()	Giống execl(), nhưng tìm lệnh trong biến môi trường PATH
- execv()	Dùng mảng tham số
- execvp()	Kết hợp execv() và tìm đường dẫn từ PATH

- setevn(): dùng để thiết lập biến môi trường
- getevn(): lấy giá trị biến môi trường



# Thay thế Mã thực thi và Tương tác với Môi trường

Bài tập này giúp tìm hiểu cách một tiến trình có thể thay thế hoàn toàn mã lệnh đang chạy của nó bằng một chương trình khác, cũng như cách truyền thông tin qua biến môi trường.

## 📝 Yêu cầu

1. **Tiến trình cha**:
   - Thiết lập một biến môi trường, ví dụ: `MY_COMMAND=ls`.

2. **Tiến trình con**:
   - Đọc giá trị của biến môi trường `MY_COMMAND`.
   - Dựa vào giá trị đọc được, sử dụng một hàm trong họ `exec()` (ví dụ: `execlp()`) để thực thi lệnh tương ứng (ví dụ: `ls`, `date`,...).
