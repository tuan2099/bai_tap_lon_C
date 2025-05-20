#include "user.h"
#include "wallet.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

void syncWalletsWithUsers(const vector<User>& users, vector<Wallet>& wallets) {
    unordered_map<string, bool> existingWallets;
    for (const auto& wallet : wallets) {
        existingWallets[wallet.walletID] = true;
    }
    bool walletsChanged = false;
    for (const auto& user : users) {
        if (!user.walletID.empty() && existingWallets.find(user.walletID) == existingWallets.end()) {
            wallets.emplace_back(user.walletID, 0.0);
            cout << "Đã tạo ví mới " << user.walletID << " cho người dùng " << user.username << endl;
            walletsChanged = true;
        } else if (user.walletID.empty()) {
            cerr << "Lỗi: Người dùng " << user.username << " không có Wallet ID hợp lệ." << endl;
        }
    }
    if (walletsChanged) {
        saveWallets(wallets, "wallets.txt");
    }
}

void ensureDefaultAdmin(vector<User>& users, vector<Wallet>& wallets) {
    for (const auto& user : users) {
        if (user.username == "admin") return;
    }
    cout << "Admin mặc định chưa tồn tại. Đang tạo admin..." << endl;
    string adminPass = "admin123";
    string adminOathKey = generateOATHSecretKey();
    string hashedAdminPass = hashPassword(adminPass);
    if (!hashedAdminPass.empty()) {
        users.emplace_back("admin", hashedAdminPass, "Quản Trị Viên", "admin@gmail.com", "000000000", "admin", "W_admin_0", adminOathKey, true);
        cout << "Đã tạo tài khoản admin mặc định:\n  Username: admin\n  Password: " << adminPass << "\n  OATH Secret (Base32): " << adminOathKey << endl;
        wallets.emplace_back("W_admin_0", 0.0);
        saveWallets(wallets, "wallets.txt");
        saveUsers(users, "users.txt");
    } else {
        cerr << "Lỗi: Không thể tạo mật khẩu cho admin mặc định." << endl;
    }
}

int showMainMenu() {
    cout << "\n========== HỆ THỐNG QUẢN LÝ ==========" << endl;
    cout << "1. Đăng nhập" << endl;
    cout << "2. Đăng ký (Người dùng tự đăng ký)" << endl;
    cout << "3. Thoát" << endl;
    cout << "======================================" << endl;
    cout << "Chọn chức năng: ";
    int choice;
    cin >> choice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }
    return choice;
}

void handleUserMenu(User* currentUser, vector<User>& users, vector<Wallet>& wallets) {
    while (currentUser != nullptr) {
        cout << "\n---------- Chào " << currentUser->username << " (" << currentUser->role << ") ----------" << endl;
        cout << "1. Xem thông tin cá nhân" << endl;
        cout << "2. Cập nhật thông tin cá nhân" << endl;
        cout << "3. Đổi mật khẩu" << endl;
        cout << "4. Quản lý ví điểm thưởng" << endl;
        int nextOption = 5;
        if (currentUser->role == "admin") {
            cout << nextOption++ << ". Xem danh sách người dùng" << endl;
            cout << nextOption++ << ". Tạo tài khoản mới (cho người dùng)" << endl;
            cout << nextOption++ << ". Chỉnh sửa tài khoản người dùng" << endl;
        }
        cout << nextOption << ". Đăng xuất" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Chọn chức năng: ";
        int userChoice;
        cin >> userChoice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (userChoice == 1) {
            cout << "\n--- Thông tin cá nhân ---" << endl;
            cout << "Username: " << currentUser->username << endl;
            cout << "Full Name: " << currentUser->fullName << endl;
            cout << "Email: " << currentUser->email << endl;
            cout << "Phone: " << currentUser->phone << endl;
            cout << "Role: " << currentUser->role << endl;
            cout << "Wallet ID: " << currentUser->walletID << endl;
        } else if (userChoice == 2) {
            updateProfileUser(*currentUser, users);
        } else if (userChoice == 3) {
            changePassword(*currentUser, users);
        } else if (userChoice == 4) {
            while (true) {
                cout << "\n--- Quản lý Ví Điểm Thưởng ---" << endl;
                cout << "1. Xem thông tin ví & lịch sử giao dịch" << endl;
                cout << "2. Chuyển điểm" << endl;
                cout << "3. Quay lại menu trước" << endl;
                cout << "Chọn: ";
                int walletChoice;
                cin >> walletChoice;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                if (walletChoice == 1) {
                    viewInForWallet(*currentUser, wallets);
                } else if (walletChoice == 2) {
                    transferPointsWallet(*currentUser, users, wallets);
                } else if (walletChoice == 3) {
                    break;
                }
            }
        } else {
            int logoutOption = 5;
            if (currentUser->role == "admin") {
                if (userChoice == logoutOption++) {
                    ViewInfoUserbyAdmin(users);
                } else if (userChoice == logoutOption++) {
                    User newUser = createNewUser(users, true);
                    if (!newUser.username.empty()) {
                        bool found = false;
                        for (const auto& w : wallets) {
                            if (w.walletID == newUser.walletID) {
                                found = true;
                                break;
                            }
                        }
                        if (!found && !newUser.walletID.empty()) {
                            wallets.emplace_back(newUser.walletID, 0.0);
                            saveWallets(wallets, "wallets.txt");
                        }
                        saveUsers(users, "users.txt");
                    }
                } else if (userChoice == logoutOption++) {
                    EditUserWithAdminRole(users);
                }
            }
            if (userChoice == logoutOption) {
                currentUser = nullptr;
                break;
            }
        }
    }
}

int main() {
    srand(time(0));
    vector<User> users = loadUsers("users.txt");
    vector<Wallet> wallets = getInfoWalet("wallets.txt");

    syncWalletsWithUsers(users, wallets);
    ensureDefaultAdmin(users, wallets);

    while (true) {
        int choice = showMainMenu();
        if (choice == 1) {
            User* currentUser = login(users);
            if (currentUser) handleUserMenu(currentUser, users, wallets);
        } else if (choice == 2) {
            User newUser = createNewUser(users);
            if (!newUser.username.empty()) {
                bool exists = false;
                for (const auto& w : wallets) {
                    if (w.walletID == newUser.walletID) {
                        exists = true;
                        break;
                    }
                }
                if (!exists && !newUser.walletID.empty()) {
                    wallets.emplace_back(newUser.walletID, 0.0);
                    saveWallets(wallets, "wallets.txt");
                    cout << "Đã tự động tạo ví " << newUser.walletID << " cho bạn." << endl;
                }
                saveUsers(users, "users.txt");
            } else {
                cout << "Đăng ký thất bại." << endl;
            }
        } else if (choice == 3) {
            cout << "Tạm biệt!" << endl;
            break;
        } else {
            cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << endl;
        }
    }
    return 0;
}
