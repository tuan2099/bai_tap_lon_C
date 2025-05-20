#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>

class User;

/// @brief Lớp đại diện cho ví điểm thưởng của người dùng
class Wallet {
public:
    std::string walletID;                         ///< Mã định danh của ví
    double balance;                               ///< Số dư điểm hiện tại trong ví
    std::vector<std::string> transactionLog;      ///< Danh sách lịch sử giao dịch

    /// @brief Khởi tạo ví mới
    /// @param id Mã ví
    /// @param b Số dư ban đầu (mặc định = 0.0)
    Wallet(std::string id, double b = 0.0);
};

/// @brief Ghi danh sách ví ra file
/// @param wallets Danh sách các ví
/// @param relativeFilename Tên file (tương đối trong thư mục dữ liệu)
void saveWallets(const std::vector<Wallet>& wallets, const std::string& relativeFilename);

/// @brief Tải danh sách ví từ file (hoặc file sao lưu nếu cần)
/// @param relativeFilename Tên file (tương đối trong thư mục dữ liệu)
/// @return Danh sách các ví đã nạp
std::vector<Wallet> getInfoWalet(const std::string& relativeFilename);

/// @brief Thực hiện chuyển điểm giữa hai ví người dùng
/// @param sender Người gửi điểm
/// @param users Danh sách người dùng để tra cứu thông tin liên quan
/// @param wallets Danh sách các ví để cập nhật số dư và ghi log
void transferPointsWallet(User& sender, std::vector<User>& users, std::vector<Wallet>& wallets);

/// @brief Hiển thị thông tin ví của một người dùng cụ thể
/// @param user Người dùng cần hiển thị ví
/// @param wallets Danh sách ví để tìm ví tương ứng
void viewInForWallet(const User& user, const std::vector<Wallet>& wallets);

#endif // WALLET_H

