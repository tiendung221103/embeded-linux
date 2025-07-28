- Các kiến thức cần nắm được:
**XÂY DỰNG VÀ SỬ DỤNG THƯ VIỆN**
B1: biên dịch thành các file obj từ các file .c
B2: tạo thư viện cần dùng (static/shared)
B3: liên kết các file obj với thư viện tương ứng
B4: dọn dẹp các file không cần thiết (.o,.a,.so)

**SỬ DỤNG MAKEFILE**
target: dependencies
<TAB>command

- Xây dựng và Sử dụng Thư viện Static(.a) & Shared(.so) trong C

- Yêu cầu: Xây dựng Thư viện Tiện ích Xử lý Chuỗi (strutils)

- Để chạy code, tạo thêm các folder bin để chứa file thực thi, folder obj để chứa các file object,
folder lib để chứa các file thư viện
- Lưu ý khi chạy: Để chạy tệp shared-linked, bạn cần chỉ cho hệ điều hành nơi tìm file .so. (Gợi ý dùng biến môi trường LD_LIBRARY_PATH)