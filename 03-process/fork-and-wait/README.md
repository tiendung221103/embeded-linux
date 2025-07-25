# Khởi tạo và Thu dọn Tiến trình

Bài tập này minh họa vòng đời cơ bản của một tiến trình trong hệ điều hành Unix/Linux: được tạo ra, thực thi, kết thúc và được tiến trình cha chờ đợi.

## 📝 Yêu cầu

1. Tạo một tiến trình con bằng cách sử dụng `fork()`.
2. Trong **tiến trình cha**:
   - In ra PID của chính nó và PID của tiến trình con.
   - Sử dụng `wait()` để chờ tiến trình con kết thúc.
   - Dùng `WIFEXITED()` và `WEXITSTATUS()` để kiểm tra tiến trình con kết thúc bình thường hay không và in ra mã thoát.
3. Trong **tiến trình con**:
   - In ra PID của chính nó.
   - Gọi `exit()` với một mã thoát cụ thể (ví dụ: `exit(10)`).



