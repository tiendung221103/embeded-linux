#   Tạo và Chạy Luồng Cơ bản

Bài tập này giới thiệu các thao tác nền tảng với luồng trong lập trình C sử dụng thư viện `pthread`. Mục tiêu là tạo và quản lý luồng, cũng như sử dụng `pthread_join()` để đồng bộ luồng với chương trình chính.

## 📝 Yêu cầu

1. Viết chương trình C tạo **hai luồng**.
2. Mỗi luồng, khi bắt đầu thực thi, sẽ in ra một thông báo cùng với **ID của luồng**
3. Luồng chính sử dụng `pthread_join()` để **chờ cả hai luồng kết thúc** trước khi chương trình kết thúc.

