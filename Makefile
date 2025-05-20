
# Makefile - Tệp cấu hình tự động biên dịch và liên kết chương trình hệ thống quản lý
# File này được sử dụng để định nghĩa quy trình biên dịch, liên kết, và dọn dẹp cho project.

# Trình biên dịch C++ được sử dụng
CXX = g++

# Các tùy chọn cấu hình trình biên dịch:
CXXFLAGS = -Iinclude -std=c++11 -Wall -Wextra -g

# Các tùy chọn liên kết thư viện:
LDFLAGS = -lssl -lcrypto

# Đường dẫn thư mục chứa mã nguồn
SRC_DIR = src

# Tên của tệp thực thi sẽ được tạo
EXECUTABLE = product

# Danh sách các tệp mã nguồn chính
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/user.cpp $(SRC_DIR)/wallet.cpp

# Danh sách các tệp đối tượng (.o) tương ứng
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

# Quy tắc tạo tệp thực thi
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)
	@echo "Quá trình biên dịch hoàn tất. Khởi chạy chương trình với lệnh: ./$(EXECUTABLE)"

# Quy tắc biên dịch từng tệp mã nguồn thành tệp đối tượng
$(SRC_DIR)/main.cpp.o: $(SRC_DIR)/main.cpp include/user.h include/wallet.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(SRC_DIR)/main.cpp.o

$(SRC_DIR)/user.cpp.o: $(SRC_DIR)/user.cpp include/user.h include/wallet.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/user.cpp -o $(SRC_DIR)/user.cpp.o

$(SRC_DIR)/wallet.cpp.o: $(SRC_DIR)/wallet.cpp include/wallet.h include/user.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/wallet.cpp -o $(SRC_DIR)/wallet.cpp.o

# Quy tắc làm sạch thư mục, xóa các tệp đối tượng và tệp thực thi
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(SRC_DIR)/*.o
	@echo "Hoàn thành dọn dẹp hệ thống."

.PHONY: all clean
