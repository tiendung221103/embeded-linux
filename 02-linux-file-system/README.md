# Trình kiểm tra Siêu dữ liệu File
## Mở đầu
Hệ thống tệp trong Linux không chỉ lưu trữ nội dung file mà còn lưu trữ rất nhiều thông tin bổ sung gọi là siêu dữ liệu (metadata). Những thông tin này được lưu trong cấu trúc inode của hệ thống file và cho phép người dùng hoặc chương trình truy cập các thuộc tính quan trọng của tệp tin mà không cần mở nội dung thực tế.

Việc hiểu và trích xuất siêu dữ liệu là bước quan trọng trong các công cụ quản trị hệ thống, phân tích nhật ký, kiểm tra bảo mật, hoặc khi cần kiểm chứng quyền truy cập và trạng thái của tệp.

## 1. Tổng quan

**Yêu cầu bài toán**

Viết chương trình `filestat.c`

Chương trình có nhiệm vụ hiển thị thông tin siêu dữ liệu của một tệp tin hoặc thư mục, dựa vào đường dẫn được cung cấp qua dòng lệnh. Yêu cầu chi tiết như sau:

* Chương trình phải nhận **một tham số duy nhất** từ dòng lệnh – đó là đường dẫn đến **một tệp hoặc thư mục**.
* Nếu không có tham số nào được cung cấp, hãy hiển thị thông báo hướng dẫn sử dụng và thoát chương trình.

**Ví dụ:**

```bash
Usage: ./filestat <file_path>
```

* Sử dụng lệnh gọi hệ thống `lstat()` để lấy thông tin metadata của đối tượng tại đường dẫn được nhập.
* Lưu kết quả vào một biến kiểu `struct stat`.

- Hiển thị các thông tin sau:

* **File Path:** Hiển thị lại đường dẫn mà người dùng đã nhập.

* **File Type:** Xác định và in ra **loại của đối tượng**, ít nhất gồm 3 loại:

  * `"Regular File"`
  * `"Directory"`
  * `"Symbolic Link"`

  **Gợi ý:** Sử dụng các macro từ `<sys/stat.h>` như:

  * `S_ISREG(st_mode)` – kiểm tra tệp thường
  * `S_ISDIR(st_mode)` – kiểm tra thư mục
  * `S_ISLNK(st_mode)` – kiểm tra symbolic link

* **Size:** Kích thước của tệp (lấy từ trường `st_size`), hiển thị đơn vị là `"bytes"`.

* **Last Modified:** Thời gian sửa đổi lần cuối (lấy từ trường `st_mtime`)

  * Chuyển đổi giá trị timestamp này sang chuỗi định dạng dễ đọc (ngày-tháng-năm giờ-phút-giây).
  * **Gợi ý:** Sử dụng các hàm `ctime()` hoặc `strftime()` từ thư viện `<time.h>` để định dạng thời gian.

---

## 2.Cấu trúc thư mục
```bash
.
├── filestat.c
├── Makefile
└── README.md
```
**Vai trò của từng file:**
- filestat.c: chương trình chính có nhiệm vụ đọc và hiển thị các thông tin quan trọng của một tệp bất kỳ
- Makefile: Tự động hóa quá trình biên dịch và dọn dẹp file. 

## 3. Cách biên dịch và chạy

### 3.1. Biên dịch

- Mở terminal và di chuyển đến thư mục chứa dự án
- Biên dịch chương trình
```bash
make all
```

### 3.2. Chạy chương trình

Để chạy chương trình:
```bash
./app
```

**Kết quả đầu ra mẫu:**

```bash
File Path     : /home/dung/embeded-linux/01-general-knowleague/README.md
File Type     : Regular File
Size          : 2906 bytes
Last Modified : 2025-07-29 14:29:41
```

- Để dọn dẹp các file thực thi:
```bash
make clean
```

## 4. Kết luận

Chương trình `filestat` giúp bạn hiểu cách truy xuất và hiển thị siêu dữ liệu của tệp tin bằng cách sử dụng lời gọi hệ thống `lstat()`. Qua đó, bạn làm quen với thao tác xử lý file trong C và biết cách định dạng thông tin để hiển thị rõ ràng, dễ hiểu cho người dùng.

Ngoài ra còn có các kiến thức khác như:
- Các system call: open(), read(), write(), lseek(), close().
- File Locking: flock(), fcntl().