# Tái tạo toán tử `|` (Pipe) trong Shell bằng C

## 1. Tổng quan

Toán tử `|` trong shell (pipe) cho phép chuyển **đầu ra (stdout)** của một lệnh thành **đầu vào (stdin)** của một lệnh khác. Ví dụ:

```bash
ls -l | wc -l
```
- `ls -l`: liệt kê chi tiết các tệp/thư mục.

- `|`: chuyển kết quả đó sang cho...

- `wc -l`: đếm số dòng (→ số tệp).

Mục tiêu: Viết một chương trình C thực hiện chính xác luồng xử lý trên thông qua cơ chế pipe và process.

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```

## 3. Mô tả hoạt động

Luồng xử lý chính:
1. Tạo pipe: sử dụng `pipe(fd)`, tạo hai đầu: `fd[0]` (đọc), `fd[1]` (ghi).

2. Tạo tiến trình con 1:

Gọi `fork()`.

Dùng `dup2(fd[1], STDOUT_FILENO)` → chuyển hướng stdout vào đầu ghi của pipe.

Gọi `execlp("ls", "ls", "-l", NULL)` để thực thi `ls -l`.

3. Tạo tiến trình con 2:

Gọi `fork()`.

Dùng `dup2(fd[0], STDIN_FILENO)` → chuyển stdin thành đầu đọc từ pipe.

Gọi `execlp("wc", "wc", "-l", NULL)` để thực thi `wc -l`.

4. Tiến trình cha:

Đóng cả hai đầu pipe (`close(fd[0])`, `close(fd[1])`).

Gọi `wait()` hai lần để đợi cả hai tiến trình con kết thúc.


## 4. Biên dịch và chạy

Biên dịch:
```bash
make
```

Chạy chương trình:
```bash
app
```

Kết quả mong đợi:
Chương trình sẽ in ra một số nguyên duy nhất – tương đương với kết quả của `ls -l | wc -l` trong terminal (tức là số dòng mà `ls -l` in ra).

## 5. Giải thích kỹ thuật

🧠 dup2(fd, STDOUT_FILENO / STDIN_FILENO)
- `dup2()` chuyển hướng đầu vào/ra của tiến trình con:

    - Với `ls -l`: stdout → pipe.

    - Với `wc -l`: stdin ← pipe.

Sau đó `execlp()` sẽ dùng đúng luồng I/O này.

💡 Tại sao phải `close()` trong tiến trình cha?
- Nếu không đóng đầu ghi của pipe trong cha, `wc -l` sẽ chờ mãi vì pipe chưa báo EOF.

- Đóng cả hai đầu giúp tiến trình con biết khi nào kết thúc dữ liệu.


## 6. Kết luận

Bài tập này giúp bạn hiểu sâu về:

- Giao tiếp giữa tiến trình bằng pipe.

- Sử dụng `fork()` + `exec()` để mô phỏng shell.

- Cách hoạt động thật sự của toán tử `|` trong shell Linux.

Kỹ thuật này là nền tảng cho việc xây dựng shell tùy biến, trình giám sát tiến trình, hoặc hệ thống xử lý dữ liệu phân tán đơn giản.