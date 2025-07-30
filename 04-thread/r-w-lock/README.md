# Lập Trình Luồng - Bài 4: Tối ưu hóa Truy cập với Read-Write Lock

## 1. Tổng quan

Trong các hệ thống có **tần suất đọc dữ liệu cao hơn nhiều so với ghi**, việc sử dụng Mutex thông thường có thể dẫn đến **tắc nghẽn không cần thiết**. Bài tập này giới thiệu cơ chế khóa chuyên biệt **Read-Write Lock** (`pthread_rwlock_t`) giúp:

- Cho phép **nhiều Reader cùng truy cập** tài nguyên một lúc.
- Đảm bảo chỉ có **một Writer** được phép ghi tại một thời điểm.
# Cách sử dụng hàm:
- Khai báo và khởi tạo pthread_rwlock_t với pthread_rwlock_init().
- Sử dụng pthread_rwlock_rdlock() để khóa đọc cho các Reader (cho phép nhiều Reader đọc cùng lúc).
- Sử dụng pthread_rwlock_wrlock() để khóa ghi cho các Writer (chỉ một Writer ghi tại một thời điểm).
- Sử dụng pthread_rwlock_unlock() để mở khóa sau khi hoàn thành.


---

## 2. Cấu trúc thư mục

```bash
.
├── main.c
├── Makefile
└── README.md
```

**Vai trò các file:**

- `main.c`: Mô phỏng bài toán nhiều Reader/Ghi truy cập tài nguyên dùng Read-Write Lock.
- `Makefile`: Biên dịch và dọn dẹp tự động.
- `README.md`: Tài liệu mô tả, hướng dẫn bài tập.

---

## 3. Mô tả chương trình

- Sử dụng một **biến nguyên toàn cục** làm tài nguyên dùng chung (vd: `int resource = 0`).
- Tạo **5 luồng Reader** và **2 luồng Writer**.

### Reader:
- Mỗi Reader yêu cầu khóa đọc (`pthread_rwlock_rdlock`).
- Đọc giá trị hiện tại của tài nguyên.
- In giá trị và giả lập xử lý bằng `sleep`.
- Giải phóng khóa bằng `pthread_rwlock_unlock`.

### Writer:
- Mỗi Writer yêu cầu **khóa ghi** (`pthread_rwlock_wrlock`).
- Tăng giá trị tài nguyên thêm 1.
- In kết quả và giả lập xử lý bằng `sleep`.
- Giải phóng khóa.

---

## 4. Cách biên dịch và chạy

### 4.1. Biên dịch chương trình

```bash
make
```

### 4.2. Chạy chương trình

```bash
./app
```

Kết quả mẫu:

```bash
Reader 1 read resource: 0
Reader 4 read resource: 0
Reader 3 read resource: 0
Reader 5 read resource: 0
Reader 2 read resource: 0
Writer 1 wrote resource: 1
Writer 2 wrote resource: 2
Reader 1 read resource: 2
Reader 5 read resource: 2
Reader 2 read resource: 2
Reader 4 read resource: 2
Reader 3 read resource: 2
Writer 1 wrote resource: 3
Reader 1 read resource: 3
Writer 2 wrote resource: 4
Reader 2 read resource: 4
...
```

### 4.3. Dọn dẹp

```bash
make clean
```

---

## 5. Giải thích
**Khi nào dùng Read-Write Lock?**
- Khi có nhiều luồng đọc và ít luồng ghi, RWLock giúp tối ưu hiệu suất hơn so với dùng `pthread_mutex_t`.

- RWLock có hai chế độ:

- Khóa đọc (rdlock): nhiều Reader có thể chạy song song.

- Khóa ghi (wrlock): Writer chạy độc quyền, chặn cả Reader và Writer khác.

**Đồng bộ đúng cách:**
- Tất cả Reader/Writer phải dùng cùng một biến `pthread_rwlock_t` để đồng bộ truy cập.

- Việc sử dụng không đúng có thể gây ra race condition hoặc deadlock.

---

## 6. Kết luận

Bài tập này giúp làm quen với cơ chế Read-Write Lock, hữu ích trong những hệ thống có lượng đọc lớn. Đây là một công cụ mạnh để tối ưu hiệu năng trong lập trình đa luồng và hệ thống.
