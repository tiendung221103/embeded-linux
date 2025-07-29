# Xây dựng và Sử dụng Thư viện Static & Shared trong C

## 1. Tổng quan

Trong lập trình C, khi một dự án lớn dần, việc tổ chức mã nguồn thành các module có thể tái sử dụng là cực kỳ quan trọng. Thư viện (library) là cách chúng ta đóng gói một tập hợp các hàm liên quan để chia sẻ giữa nhiều dự án khác nhau mà không cần sao chép mã nguồn.

* **Static Library (.a):** Mã của thư viện được sao chép và nhúng trực tiếp vào tệp thực thi tại thời điểm biên dịch. Tệp thực thi sẽ lớn hơn nhưng có thể chạy độc lập.
* **Shared Library (.so ):** Mã của thư viện không được nhúng vào tệp thực thi. Thay vào đó, hệ điều hành sẽ tải thư viện này tại thời điểm chạy và nhiều chương trình có thể dùng chung thư viện này.

Dự án này hướng dẫn cách xây dựng thư viện tiện ích xử lý chuỗi (strutils) dưới hai dạng static và shared, sử dụng trong chương trình chính và tự động hóa quá trình build bằng Makefile.

---

## 2. Cấu trúc thư mục

```
.
├── inc
│   └── strutils.h
├── Makefile
├── README.md
└── src
    ├── bstrutils.c
    └── main.c
```

**Vai trò của từng file:**

* `strutils.h`: Định nghĩa các hàm mà thư viện sẽ cung cấp.
* `bstrutils.c`: Mã nguồn triển khai các hàm trong `strutils.h`.
* `main.c`: Chương trình chính để kiểm thử các hàm trong thư viện.
* `Makefile`: Tự động hóa quá trình biên dịch và dọn dẹp file.

---

## 3. Cách biên dịch và chạy

### 3.1. Biên dịch

* Mở Terminal và di chuyển đến thư mục gốc của dự án.
* Tạo thư mục build (nếu chưa có):

```bash
make build
```

* Biên dịch toàn bộ thư viện và chương trình:

```bash
make all
```

### 3.2. Chạy chương trình

Sau khi biên dịch xong, di chuyển đến thư mục `bin` để chạy:

* Chạy với Static Library:

```bash
./statically-linked
```

* Chạy với Shared Library (cần chỉ rõ đường dẫn chứa `.so`):

```bash
LD_LIBRARY_PATH=../lib/shared ./shared-linked
```

**Kết quả đầu ra mẫu:**

```bash
Original str1: '  Hello World!  '
Trimmed str1: 'Hello World!'
Original str3: 'Test'
Reversed str3: 'tseT'
Converted '12345' to number: 12345
```

* Dọn dẹp các file build:

```bash
make clean
```

---

## 6. Kết luận

Sau bước này, bạn đã xây dựng thành công thư viện `strutils` dưới dạng static và shared, đồng thời biết cách biên dịch và liên kết chương trình sử dụng thư viện này. Việc tách riêng mã nguồn và đóng gói thư viện giúp mã dễ bảo trì, tái sử dụng và mở rộng trong các dự án sau.

