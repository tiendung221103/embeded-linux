# Thao Tác Với Tiến Trình - Bài 3: Khảo sát các Trạng thái Tiến trình Đặc biệt

## 1. Tổng quan

Bài tập này giúp tìm hiểu hai trạng thái đặc biệt của tiến trình trong hệ điều hành Linux là **Zombie** và **Orphan**. Đây là hai trạng thái quan trọng khi quản lý tiến trình và có liên quan đến việc thu dọn tài nguyên sau khi tiến trình kết thúc.

---

## 2. Cấu trúc thư mục

```bash
.
├── Makefile
├── orphan.c
├── README.md
└── zombie.c
```

**Vai trò các file:**

- `zombie.c`: Tạo tiến trình Zombie bằng cách không gọi `wait()` trong tiến trình cha.
- `orphan.c`: Tạo tiến trình Orphan bằng cách cho tiến trình cha thoát sớm.
- `Makefile`: Tự động hóa biên dịch và dọn dẹp.
- `README.md`: Hướng dẫn sử dụng và mô tả bài tập.

---

## 3. Mô tả chương trình

### 3.1. Tạo tiến trình Zombie

- Tiến trình con thoát ngay lập tức với `exit()`.
- Tiến trình cha không gọi `wait()` mà `sleep()` trong một khoảng thời gian (vd: 30 giây).
- Trong thời gian đó, tiến trình con tồn tại ở trạng thái Zombie (chưa được thu dọn).
- Dùng lệnh `ps aux` hoặc `ps -elf` để quan sát trạng thái `<defunct>` của tiến trình con.

### 3.2. Tạo tiến trình Orphan

- Tiến trình cha thoát ngay sau khi tạo tiến trình con.
- Tiến trình con sẽ `sleep()` trong một khoảng thời gian.
- Trong thời gian đó, quan sát PID cha (`PPID`) của tiến trình con thay đổi về PID của tiến trình `init` (thường là `1` hoặc `systemd`).
- Dùng `ps -o pid,ppid,cmd` để kiểm tra sự thay đổi.

---

## 4. Cách biên dịch và chạy

### 4.1. Biên dịch

```bash
make
```

### 4.2. Chạy tiến trình Zombie

```bash
./app1
```

Sau đó mở 1 Terminal khác và chạy:
```bash
ps -aux | grep app
```

Kết quả đầu ra mẫu:

```bash
Parent (PID: 4446) is not calling wait(). Sleeping for 30 seconds...
Child (PID: 4447) is terminating.
```

```bash
dung        4446  0.0  0.0   2776  1408 pts/4    S+   09:11   0:00 ./app1
dung        4447  0.0  0.0      0     0 pts/4    Z+   09:11   0:00 [app1] <defunct>
dung        4495  0.0  0.0  12088  2688 pts/5    S+   09:11   0:00 grep --color=auto app1
```

### 4.3. Chạy tiến trình Orphan

```bash
app2
```

Kết quả đầu ra mẫu:
```bash
Parent process (PID: 4650) is terminating.
Child process (PID: 4651), new parent (PPID): 1
```

---

## 5. Giải thích

- Tiến trình Zombie: Là tiến trình đã kết thúc nhưng chưa được tiến trình cha gọi wait() để thu dọn. Nó vẫn chiếm một mục trong bảng tiến trình nhằm lưu giữ mã thoát cho cha xử lý.

- Tiến trình Orphan: Là tiến trình con mà cha đã thoát trước. Khi đó, tiến trình con sẽ được tiến trình init (PID 1) nhận nuôi và tiếp tục thực thi bình thường.

---

## 6. Kết luận

Bài tập giúp quan sát trực tiếp hai trạng thái thường gặp trong quản lý tiến trình:

- Hiểu tầm quan trọng của việc gọi wait() để thu dọn tiến trình con.

- Hiểu cách hệ thống tự động xử lý tiến trình mồ côi bằng cơ chế nhận nuôi của init.