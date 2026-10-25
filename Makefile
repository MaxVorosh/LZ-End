CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude -Iexternal/lz-end-toolkit/include -Iexternal/lz-end-toolkit/extras/in-ram-parser/include
BUILD_DIR = build
TARGET = $(BUILD_DIR)/triplets
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/utils.o $(BUILD_DIR)/lzend_writer.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/main.o: main.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/lzend_writer.o: src/lzend_writer.cpp include/lzend_writer.hpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c src/lzend_writer.cpp -o $(BUILD_DIR)/lzend_writer.o

$(BUILD_DIR)/utils.o: src/utils_shim.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c src/utils_shim.cpp -o $(BUILD_DIR)/utils.o

test: $(BUILD_DIR)/test_writer

$(BUILD_DIR)/test_writer: tests/test_writer.cpp src/lzend_writer.cpp src/utils_shim.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ tests/test_writer.cpp src/lzend_writer.cpp src/utils_shim.cpp

clean:
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
