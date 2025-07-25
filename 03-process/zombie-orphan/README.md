# Khảo sát các Trạng thái Tiến trình Đặc biệt

Bài tập này giúp quan sát và phân tích hai trạng thái tiến trình đặc biệt thường gặp trong quản lý tiến trình trên hệ điều hành Linux: **Zombie** và **Orphan**.

## 📝 Yêu cầu

### 1. Tạo tiến trình Zombie:
- Viết chương trình mà tiến trình **con thoát ngay lập tức**, nhưng tiến trình **cha không gọi `wait()`**, mà chỉ `sleep()` trong một khoảng thời gian dài.
- Sử dụng lệnh `ps` trên terminal để quan sát trạng thái `<defunct>` của tiến trình con.

### 2. Tạo tiến trình Orphan:
- Viết chương trình mà tiến trình **cha thoát ngay sau khi tạo tiến trình con**.
- Tiến trình con sẽ `sleep()` một lúc và trong thời gian đó, liên tục in ra **PID của tiến trình cha (PPID)**.
- Quan sát sự thay đổi của **PPID**, thường sẽ trở thành PID của tiến trình `init` hoặc `systemd`.

### 3. Báo cáo:
- Giải thích: Vì sao các trạng thái **Zombie** và **Orphan** lại xuất hiện?
- Ý nghĩa của chúng trong quản lý tiến trình của hệ điều hành Linux là gì?
