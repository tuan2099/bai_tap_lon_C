#include "wallet.h"
#include "user.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

using namespace std;

const string DATA_DIR = "data/"; // wallets.txt - file lưu thông tin

Wallet::Wallet(string id, double b) : walletID(id), balance(b) {}

// Lưu thông tin ví vào file
void saveWallets(const vector<Wallet>& wallets, const string& relativeFilename) {
    string filename = DATA_DIR + relativeFilename;
    string backupFilename = filename + ".bak";
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Loi: Khong the mo file " << filename << " de ghi (Dam bao thu muc 'data/' ton tai)." << endl;
        return;
    }
    for (const auto& wallet : wallets) {
        file << wallet.walletID << "," << wallet.balance << "\n";
        for (const auto& log : wallet.transactionLog) {
            file << "LOG:" << log << "\n";
        }
    }
    file.close();
    string backupCmd = "cp \"" + filename + "\" \"" + backupFilename + "\"";
#ifdef _WIN32
    backupCmd = "copy \"" + filename + "\" \"" + backupFilename + "\"";
#endif
    int result = system(backupCmd.c_str());
    if (result != 0) {
        cerr << "Canh bao: Khong the tao file backup cho " << filename << ". Ma loi: " << result << endl;
    }
}

// Hàm cho phép xem thông tin ví
void viewInForWallet(const User& user, const vector<Wallet>& wallets) {
    bool found = false;

    for (const auto& wallet : wallets) {
        if (wallet.walletID == user.walletID) {
            cout << "\n--- Chi tiết ví của người dùng: ---" << endl;
            cout << "Mã ví: " << wallet.walletID << "\n";
            cout << "Số dư hiện tại: " << fixed << setprecision(2) << wallet.balance << " điểm\n";
            cout << "Lịch sử giao dịch:\n";

            if (wallet.transactionLog.empty()) {
                cout << "  (Không có giao dịch nào được ghi nhận)\n";
            } else {
                for (size_t i = 0; i < wallet.transactionLog.size(); ++i) {
                    cout << "  - " << wallet.transactionLog[wallet.transactionLog.size() - 1 - i] << "\n";
                }
            }

            cout << "--------------------" << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cerr << "Lỗi: Không thể tìm thấy ví được liên kết với tài khoản này (" << user.walletID << ")." << endl;
    }
}

// Hàm đọc thông tin ví người dùng từ file (hoặc file sao lưu )
vector<Wallet> getInfoWalet(const string& relativeFilename) {
    string filename = DATA_DIR + relativeFilename;
    string backupFilename = filename + ".bak";
    vector<Wallet> wallets;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Không thể truy cập file: " << filename << ". Đang thử sử dụng file sao lưu " << backupFilename << endl;
        ifstream backupFile(backupFilename);
        if (backupFile.is_open()) {
            file.swap(backupFile);
        } else {
            cerr << "Lỗi nghiêm trọng: Không thể tải dữ liệu ví từ cả file chính và file sao lưu. Khởi tạo danh sách ví trống." << endl;
            return wallets;
        }
    }

    string line;
    Wallet* currentWallet = nullptr;

    while (getline(file, line)) {
        if (line.rfind("LOG:", 0) == 0) {
            if (currentWallet && line.length() > 4) {
                currentWallet->transactionLog.push_back(line.substr(4));
            }
        } else {
            size_t pos = line.find(",");
            if (pos != string::npos) {
                string walletID = line.substr(0, pos);
                try {
                    double balance = stod(line.substr(pos + 1));
                    wallets.emplace_back(walletID, balance);
                    currentWallet = &wallets.back();
                } catch (const invalid_argument& ia) {
                    cerr << "Dữ liệu không hợp lệ trong wallets.txt: Không phải số. Đã bỏ qua dòng: " << line << endl;
                    currentWallet = nullptr;
                } catch (const out_of_range& oor) {
                    cerr << "Giá trị số trong wallets.txt vượt quá giới hạn. Đã bỏ qua dòng: " << line << endl;
                    currentWallet = nullptr;
                }
            } else {
                cerr << "Định dạng sai trong wallets.txt: Thiếu dấu phẩy. Đã bỏ qua dòng: " << line << endl;
                currentWallet = nullptr;
            }
        }
    }

    file.close();
    return wallets; // tt ví đã đọc
}

// Hàm thực hiện chuyển điểm giữa các ví
void transferPointsWallet(User& sender, vector<User>& users, vector<Wallet>& wallets) {
    string destWalletID;
    double points;
    cout << "Nhập mã ví đích (walletID): "; //  nhập tt ví
    cin >> destWalletID;
    cout << "Nhập số điểm cần chuyển: ";
    cin >> points;

    // điểm > 0 thì mới tiếp tục
    if (points <= 0) {
        cerr << "Lỗi: Số điểm chuyển phải lớn hơn 0.\n";
        return;
    }


    Wallet* senderWallet = nullptr;
    Wallet* destWallet = nullptr;
    // lấy thông tin ví người gửi
    for (auto& wallet : wallets) {
        if (wallet.walletID == sender.walletID) senderWallet = &wallet;
        if (wallet.walletID == destWalletID) destWallet = &wallet;
    }

    if (!senderWallet) {
        cerr << "Lỗi: Không tìm thấy ví của người gửi!\n"; // ví gửi
        return;
    }
    if (!destWallet) {
        cerr << "Lỗi: Ví đích không tồn tại!\n"; // ví nhậns    
        return;
    }
    if (senderWallet == destWallet) {
        cerr << "Lỗi: Bạn không thể chuyển điểm cho chính mình.\n";
        return;
    }
    if (senderWallet->balance < points) {
        cerr << "Lỗi: Số dư không đủ! (Số dư hiện tại: " << senderWallet->balance << ")\n"; // ko đủ số dư
        return;
    }
    // cập nhật thôngtin
    destWallet->balance += points;
    senderWallet->balance -= points;
    // setting time
    time_t now = time(0);
    string dt = ctime(&now);
    dt.pop_back();

    senderWallet->transactionLog.push_back("[" + dt + "] Đã chuyển " + to_string(points) + " điểm tới " + destWallet->walletID);
    destWallet->transactionLog.push_back("[" + dt + "] Đã nhận " + to_string(points) + " điểm từ " + senderWallet->walletID);
    // lưu tt
    saveWallets(wallets, "wallets.txt");
    cout << "Chuyển điểm thành công!\n";
}




