# Xây dựng và Sử dụng Thư viện Static & Shared trong C
1. **Tổng quan**
Trong lập trình C, khi một dự án lớn dần, việc tổ chức mã nguồn thành các module có thể tái sử dụng là cực kỳ quan trọng. Thư viện (library) là cách chúng ta đóng gói một tập hợp các hàm liên quan để chia sẻ giữa nhiều dự án khác nhau mà không cần sao chép mã nguồn.
Static Library (.a): Mã của thư viện được sao chép và nhúng trực tiếp vào tệp thực thi của bạn tại thời điểm biên dịch (compile time). Tệp thực thi sẽ lớn hơn nhưng có thể chạy độc lập.
Shared Library (.so - Shared Object): Mã của thư viện không được nhúng vào tệp thực thi. Thay vào đó, hệ điều hành sẽ tải thư viện này vào bộ nhớ một lần tại thời điểm chạy (runtime) và nhiều chương trình có thể dùng chung.

2. **Cấu trúc thư mục**
```bash
.
├── inc
│   └── strutils.h
├── Makefile
├── README.md
└── src
    ├── bstrutils.c
    └── main.c
```

Vai trò của từng file:
    - strutils.h: Định nghĩa các hàm mà thư viện sẽ cung cấp
    - bstrutils.c: Mã nguồn để triển khai các hàm đã định nghĩa trong strutils.h.
    - main.c: Chương trình chính để kiểm thử các hàm trong thư viện strutils.
    - Makefile: Biên dịch file mã nguồn và xóa các file không cần thiết

3. **Cách biên dịch và chạy**
3.1. **Biên dịch**
- Trong Terminal, di chuyển đến folder 01-general-knowleague
- Tạo các folder cần thiết
```make build```
- Build toàn bộ module
```make all```

3.2. **Chạy chương trình**
Để chạy chương trình, di chuyển đến folder bin
- Với StaticLib:
```bash./statically-linked```
- Với SharedLib cần chỉ cho hệ điều hành nơi tìm file .so:
```bashLD_LIBRARY_PATH=../lib/shared ./shared-linked ```
- Kết quả output:
``` bash
Original str1: '  Hello World!  '
Trimmed str1: 'Hello World!'
Original str3: 'Test'
Reversed str3: 'tseT'
Converted '12345' to number: 12345 
```
- Xóa các file không cần thiết(.a, .o, .so) bằng cách:
```bash make clean```

5. **Kết luận**
Sau bước này, bạn đã xây dựng thành công thư viện strutils dưới dạng static và shared, đồng thời biết cách biên dịch và liên kết chương trình sử dụng thư viện này. Việc tách riêng mã nguồn và đóng gói thư viện giúp mã dễ bảo trì, tái sử dụng và mở rộng trong các dự án sau.