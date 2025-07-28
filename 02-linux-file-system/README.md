CÁC KIẾN THỨC CẦN NẮM
- Các system call: open(), read(), write(), lseek(), close().
- File Locking: flock(), fcntl().
- Các loại file: S_ISREG, S_ISDIR, S_ISLNK
- Sử dụng lstat() để lấy thông tin của file đường dẫn


Xây dựng Tiện ích filestat - Trình kiểm tra Siêu dữ liệu File

Chương trình có thể in ra các thông tin sau: File Path, File Type(Regular File, Directory, Symbolic Link), size, last modified.

Để có thể chạy, chương trình phải nhận chính xác một tham số từ dòng lệnh, đó là đường dẫn đến một tệp hoặc thư mục.