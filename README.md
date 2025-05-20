# Hệ Thống Quản Lý Người Dùng và Ví Điểm Thưởng

## Giới thiệu thành viên tham gia:

| **Họ Tên** | **Mã sinh viên** | **Vai trò** | **Phân chia công việc** |
| --- | --- | --- | --- |
| Hắc Ngọc Hoằng | K24DTCN513 | Lập trình viên | Thiết kế cấu trúc hệ thống, lập trình các chức năng: Menu, quản lý người dùng (admin) ,tích hợp OTP,... |
| Hoàng Anh Tuân | K24DTCN544 | Lập trình viên | Thuyết trình , giới thiệu sản phẩm , thực hiện chức năng đăng nhập, đăng ký|
| Đỗ Thị Vân Anh | K24DTCN500 | Lập trình viên | Kiểm thử & chịu trách nhiệm chất lượng phần mềm |

Project : Hệ thông thống lý điểm thưởng và quản lý đăng nhập

## Cấu trúc chương trình

```
+------------------------+
|       main.cpp         |
| - main()               |
| - Menu chính           |
|   - Đăng nhập          |
|   - Đăng ký            |
|   - Thoát              |
| - Menu người dùng      |
|   - Xem thông tin      |
|   - Cập nhật thông tin |
|   - Đổi mật khẩu       |
|   - Quản lý ví         |
|   - Chức năng admin    |
+------------------------+
          |
          | Sử dụng
          v
+---------------------+       +---------------------+
|      user.h         |<----->|      wallet.h       |
| - Lớp User          |       | - Lớp Wallet        |
|   - username        |       |   - walletID        |
|   - password        |       |   - balance         |
|   - fullName        |       |   - transactionLog  |
|   - email, phone    |       | - Hàm ví            |
|   - role, walletID  |       |   - save/load       |
|   - oathSecretKey   |       |   - transferPoints  |
| - Hàm người dùng    |       |   - viewWallet      |
|   - save/load       |       |   - depositPoints   |
|   - registerUser    |       |                     |
|   - login           |       |                     |
|   - changePassword  |       |                     |
|   - updateProfile   |       |                     |
|   - viewUsers       |       |                     |
|   - adminEditUser   |       |                     |
+---------------------+       +---------------------+
          |                           |
          | Triển khai                | Triển khai
          v                           v
+------------------------+       +-----------------------+
|      user.cpp          |       |      wallet.cpp       |
| - Triển khai User      |       | - Triển khai Wallet   |
|   functions            |       |   functions           |
| - Xử lý:               |       | - Xử lý:              |
|   - Đọc/ghi tệp        |       |   - Đọc/ghi tệp       |
|   - Băm mật khẩu       |       |   - Thao tác ví       |
|   - Sinh OTP           |       |   - Nhật ký giao dịch |
|   - Quản lý người dùng |       |                       |
+------------------------+       +-----------------------+
          |                           |
          | Đọc/Ghi                   | Đọc/Ghi
          v                           v
+--------------------------+       +---------------------+
|   data/users.txt         |       |  data/wallets.txt   |
| - Lưu dữ liệu người dùng |       | - Lưu dữ liệu ví    |
| - Định dạng: CSV         |       | - Định dạng: CSV    |
| - Sao lưu: .bak          |       | - Sao lưu: .bak     |
+--------------------------+       +---------------------+

```

## Phân tích và đặc tả chức năng

### Mục tiêu hệ thống
Hệ thống quản lý điểm thưởng được thiết kế để quản lý tài khoản người dùng và ví điểm thưởng

#### Mô tả chức năng
* Hệ thống bao gồm các chức năng chính sau:
  Đăng ký tài khoản

  - Chức năng: Cho phép người dùng hoặc quản trị viên khởi tạo tài khoản mới.

  Thông tin đầu vào: Tên đăng nhập, mật khẩu (do người dùng tự nhập hoặc hệ thống sinh tự động), họ tên, email và số điện thoại.

  Kết quả đầu ra: Tài khoản mới được tạo, kèm theo ví điểm thưởng và khóa bí mật OTP để sử dụng cho xác thực hai yếu tố.

  Yêu cầu bảo mật: Mật khẩu được mã hóa bằng thuật toán SHA-256; hỗ trợ 2FA thông qua OTP.

  Đăng nhập hệ thống

  - Chức năng: Xác thực người dùng để truy cập vào hệ thống.

  Thông tin đầu vào: Tên đăng nhập, mật khẩu và mã OTP.

  Kết quả đầu ra: Đăng nhập thành công hoặc thông báo lỗi nếu xác thực thất bại.

  Yêu cầu bảo mật: So sánh mật khẩu đã băm và kiểm tra tính hợp lệ của mã OTP.

  Quản lý thông tin cá nhân

  - Chức năng: Cho phép người dùng xem và chỉnh sửa thông tin cá nhân bao gồm họ tên, email và số điện thoại.

  Thông tin đầu vào: Dữ liệu mới cần cập nhật và mã OTP xác nhận.

  Kết quả đầu ra: Thông tin được cập nhật thành công hoặc hiển thị thông báo lỗi.

  Đổi mật khẩu

  - Chức năng: Hỗ trợ người dùng thay đổi mật khẩu tài khoản.

  Thông tin đầu vào: Mật khẩu hiện tại, mật khẩu mới và mã OTP xác minh.

  Kết quả đầu ra: Mật khẩu được thay đổi thành công hoặc thông báo lỗi nếu xác thực thất bại.

  Quản lý ví điểm thưởng

  - Chức năng: Cho phép người dùng theo dõi số dư, xem lịch sử giao dịch và thực hiện chuyển điểm tới ví khác.

  Thông tin đầu vào: Số điểm cần chuyển, ID ví nhận và mã OTP xác nhận giao dịch.

  Kết quả đầu ra: Giao dịch hoàn tất hoặc hiển thị thông báo lỗi nếu thất bại.

  Quản trị người dùng (dành cho admin)

  - Chức năng: Quản trị viên có quyền truy cập danh sách người dùng, tạo tài khoản mới và chỉnh sửa thông tin tài khoản hiện có.

  Thông tin đầu vào: Tên đăng nhập và thông tin cần thay đổi.

  Kết quả đầu ra: Danh sách người dùng được hiển thị hoặc thông tin tài khoản được cập nhật.

#### Yêu cầu Hệ thống

* Yêu cầu:
  - Trình biên dịch C++: Hỗ trợ C++11.
  - Thư viện OpenSSL: Bao gồm cả header files (thường là gói "libssl-dev" hoặc "openssl-devel").
  - OATH Toolkit 

## Hướng dẫn Cài đặt và khởi động project


## Cài đặt Dependencies

- Trên Ubuntu/Debian:
```shell
sudo apt update
sudo apt install build-essential libssl-dev oathtool
```

- Trên Windows:
Sử dụng Windows Subsystem for Linux (WSL) và làm theo hướng dẫn cho Ubuntu/Debian.
  
### 3. Biên dịch chương trình

Ứng dụng đang sử dụng "Makefile" được cung cấp để biên dịch mã nguồn:
```shell
make
```
Câu lệnh này sẽ tạo ra một file thực thi có tên "product" trong thư mục gốc của chương trình.
```shell
g++ src/main.cpp -o product -std=c++11 -lssl -lcrypto
```
### 4. Chạy Chương trình
```shell
./product
```
- Lần chạy đầu tiên: Chương trình sẽ kiểm tra và nếu không có tài khoản "admin", nó sẽ tự động tạo một tài khoản "admin" với mật khẩu mặc định (admin123) và hiển thị OATH Secret Key.
  Ghi lại OATH Secret Key này và thêm vào ứng dụng xác thực của bạn (One Time Password, Google Authenticator, Microsoft Authenticator,...). Bạn sẽ cần mã OTP từ ứng dụng này để đăng nhập.
- Đăng nhập lần đầu: Đăng nhập với "admin" và mật khẩu "admin123". Nhập mã OTP từ ứng dụng xác thực. Hệ thống sẽ yêu cầu bạn đổi mật khẩu ngay lập tức.
- Lưu trữ dữ liệu: Các file "users.txt" và "wallets.txt" (cùng các file ".bak") sẽ được tạo/cập nhật trong cùng thư mục bạn chạy file "product".

## Các thao tác thực hiện
🧩 Menu chính:
- Đăng nhập: Nhập tên người dùng, mật khẩu và mã OTP để truy cập hệ thống.
- Đăng ký: Cung cấp thông tin cá nhân để tạo tài khoản mới (bao gồm tên đăng nhập, mật khẩu, họ tên, email và số điện thoại).
- Thoát: Kết thúc phiên làm việc và thoát khỏi chương trình.

👤 Menu người dùng (sau khi đăng nhập):
- Xem thông tin cá nhân: Hiển thị chi tiết tài khoản hiện tại của người dùng.
- Cập nhật thông tin cá nhân: Thay đổi họ tên, email hoặc số điện thoại (yêu cầu xác thực OTP).
- Đổi mật khẩu: Thiết lập mật khẩu mới (cần nhập mật khẩu cũ và mã OTP để xác minh).

Quản lý ví điểm thưởng:

- Xem ví & lịch sử giao dịch: Hiển thị số dư và các giao dịch gần đây.
- Chuyển điểm: Gửi điểm đến ví khác bằng cách nhập ID người nhận, số điểm và mã OTP xác thực.
- Quay lại menu trước.
- Đăng xuất: Rời khỏi tài khoản người dùng.

🛠️ Menu quản trị viên (admin):

- Xem danh sách người dùng: Liệt kê toàn bộ tài khoản trong hệ thống.
- Tạo tài khoản mới: Tạo người dùng mới với mật khẩu được hệ thống tự động sinh.
- Chỉnh sửa thông tin người dùng: Cập nhật thông tin cá nhân và vai trò của người dùng.
- Đăng xuất: Thoát khỏi tài khoản quản trị.

## Tài liệu tham khảo

- Bài tập có sử dụng các tài liệu và nguồn tham khảo:
  + OpenSSL Documentation: https://www.openssl.org/docs/
    Được sử dụng để triển khai băm mật khẩu bằng SHA-256.
  + C++ Standard Library Reference: https://en.cppreference.com/w/
    Tham khảo cho các thư viện chuẩn như ```<vector>, <string>, <fstream>,...```
  + OATH Toolkit (oathtool): https://www.nongnu.org/oath-toolkit/
    Hướng dẫn cài đặt và sử dụng oathtool cho OTP.
