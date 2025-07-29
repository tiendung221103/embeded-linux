# Thao Tác Với Tiến Trình - Bài 2: Thay thế Mã thực thi và Tương tác với Môi trường

## 1. Tổng quan

Bài tập này giúp tìm hiểu cách một tiến trình có thể **thay thế hoàn toàn mã thực thi của mình bằng một chương trình khác** thông qua họ hàm `exec()` và **truyền thông tin giữa các tiến trình qua biến môi trường**.

Tiến trình cha sẽ thiết lập một biến môi trường, sau đó tiến trình con đọc giá trị từ biến này và thực thi chương trình tương ứng bằng cách gọi `execvp()`, `execlp()` hoặc các biến thể khác của `exec()`.


Các họ hàm exec:
- execl()	Gọi chương trình mới với danh sách tham số rời rạc
- execlp()	Giống execl(), nhưng tìm lệnh trong biến môi trường PATH
- execv()	Dùng mảng tham số
- execvp()	Kết hợp execv() và tìm đường dẫn từ PATH
- setevn(): dùng để thiết lập biến môi trường
- getevn(): lấy giá trị biến môi trường

---

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```
**Vai trò các file:**

- `main.c`: Chứa chương trình C thực hiện tạo tiến trình, truyền biến môi trường và gọi `exec`.
- `Makefile`: Hỗ trợ biên dịch nhanh và dọn dẹp.
- `README.md`: Tài liệu hướng dẫn và mô tả bài tập.


---

## 3. Mô tả chương trình

- Tiến trình cha:
  - Thiết lập một biến môi trường (ví dụ: `MY_COMMAND=ls`).
  - Gọi `fork()` để tạo tiến trình con.

- Tiến trình con:
  - Đọc biến môi trường `MY_COMMAND`.
  - Nếu biến tồn tại, sử dụng họ hàm `exec()` để thực thi chương trình tương ứng (vd: `ls`, `date`,...).
  - Nếu `exec()` thành công, tiến trình con sẽ bị **thay thế hoàn toàn** bởi chương trình mới.

---

## 4. Cách biên dịch và chạy

### 4.1. Biên dịch

Mở terminal và chạy:

```bash
make
```

### 4.2. Chạy chương trình

```bash
./app
```
Kết quả mẫu:
```bash
app  main.c  Makefile  README.md
Parent process: Child has finished.
```

### 4.3. Dọn dẹp

```bash
make clean
```
## 5. Giải thích

Khi exec() được gọi thành công trong tiến trình con:

- Không gian địa chỉ của tiến trình con bị thay thế hoàn toàn bởi chương trình mới.

- Mã lệnh ban đầu trong tiến trình con không còn tồn tại.

- Tiến trình mới sẽ kế thừa PID của tiến trình con ban đầu, nhưng không thể quay trở lại đoạn mã trước exec().

## 6. Kết luận

Bài tập giúp củng cố kiến thức về:

* Cách tiến trình con có thể thực thi chương trình khác.

* Sử dụng biến môi trường để truyền dữ liệu từ tiến trình cha sang con.

* Hiểu rõ cơ chế hoạt động của exec() và sự thay thế hoàn toàn mã thực thi của tiến trình gốc.