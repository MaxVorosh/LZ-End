CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude -Iexternal/lz-end-toolkit/include -Iexternal/lz-end-toolkit/extras/in-ram-parser/include
BUILD_DIR = build
TARGET = $(BUILD_DIR)/triplets
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/utils.o $(BUILD_DIR)/simple_writer.o $(BUILD_DIR)/simple_reader.o $(BUILD_DIR)/huffman_writer.o $(BUILD_DIR)/huffman_reader.o $(BUILD_DIR)/huffman_encoder.o $(BUILD_DIR)/huffman_decoder.o $(BUILD_DIR)/bit_reader.o $(BUILD_DIR)/bit_writer.o
GTEST_DIR = C:\Program Files\gtest\googletest-1.18.0
CXXFLAGS += -I"$(GTEST_DIR)\googletest\include"
LDFLAGS = -L"$(GTEST_DIR)\lib"

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/main.o: main.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/simple_writer.o: src/base_coder/simple_writer.cpp include/simple_coder/simple_writer.hpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c src/base_coder/simple_writer.cpp -o $(BUILD_DIR)/simple_writer.o

$(BUILD_DIR)/simple_reader.o: src/base_coder/simple_reader.cpp include/simple_coder/simple_reader.hpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c src/base_coder/simple_reader.cpp -o $(BUILD_DIR)/simple_reader.o

$(BUILD_DIR)/huffman_writer.o: src/huffman_coder/huffman_writer.cpp include/huffman_coder/huffman_writer.hpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c src/huffman_coder/huffman_writer.cpp -o $(BUILD_DIR)/huffman_writer.o

$(BUILD_DIR)/huffman_reader.o: src/huffman_coder/huffman_reader.cpp include/huffman_coder/huffman_reader.hpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c src/huffman_coder/huffman_reader.cpp -o $(BUILD_DIR)/huffman_reader.o

$(BUILD_DIR)/huffman_encoder.o: src/huffman_coder/huffman_encoder.cpp include/huffman_coder/huffman_encoder.hpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c src/huffman_coder/huffman_encoder.cpp -o $(BUILD_DIR)/huffman_encoder.o

$(BUILD_DIR)/huffman_decoder.o: src/huffman_coder/huffman_decoder.cpp include/huffman_coder/huffman_decoder.hpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c src/huffman_coder/huffman_decoder.cpp -o $(BUILD_DIR)/huffman_decoder.o

$(BUILD_DIR)/bit_reader.o: src/utils/bit_reader.cpp include/utils/bit_reader.hpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c src/utils/bit_reader.cpp -o $(BUILD_DIR)/bit_reader.o

$(BUILD_DIR)/bit_writer.o: src/utils/bit_writer.cpp include/utils/bit_writer.hpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c src/utils/bit_writer.cpp -o $(BUILD_DIR)/bit_writer.o

$(BUILD_DIR)/utils.o: src/base_coder/utils_shim.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c src/base_coder/utils_shim.cpp -o $(BUILD_DIR)/utils.o

test: $(BUILD_DIR)/test_writer

$(BUILD_DIR)/test_writer: tests/test_writer.cpp \
  src/base_coder/simple_writer.cpp src/base_coder/simple_reader.cpp \
  src/huffman_coder/huffman_writer.cpp src/huffman_coder/huffman_reader.cpp \
  src/huffman_coder/huffman_encoder.cpp src/huffman_coder/huffman_decoder.cpp \
  src/utils/bit_reader.cpp src/utils/bit_writer.cpp src/base_coder/utils_shim.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -static -L"$(GTEST_DIR)\lib" -o $@ \
	  tests/test_writer.cpp \
	  src/base_coder/simple_writer.cpp src/base_coder/simple_reader.cpp \
	  src/huffman_coder/huffman_writer.cpp src/huffman_coder/huffman_reader.cpp \
	  src/huffman_coder/huffman_encoder.cpp src/huffman_coder/huffman_decoder.cpp \
	  src/utils/bit_reader.cpp src/utils/bit_writer.cpp src/base_coder/utils_shim.cpp \
	  -lgtest -lgtest_main -lpthread

test_bit: $(BUILD_DIR)/test_bit_vector

$(BUILD_DIR)/test_bit_vector: tests/test_bit_vector.cpp src/utils/bit_vector.cpp src/utils/bit_writer.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -static -L"$(GTEST_DIR)\lib" -o $@ tests/test_bit_vector.cpp src/utils/bit_vector.cpp src/utils/bit_writer.cpp -lgtest -lgtest_main -lpthread

test_ef: $(BUILD_DIR)/test_elias_fano

$(BUILD_DIR)/test_elias_fano: tests/test_elias_fano.cpp src/utils/elias_fano_coder.cpp src/utils/bit_vector.cpp src/utils/bit_writer.cpp src/base_coder/utils_shim.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -static -L"$(GTEST_DIR)\lib" -o $@ tests/test_elias_fano.cpp src/utils/elias_fano_coder.cpp src/utils/bit_vector.cpp src/utils/bit_writer.cpp src/base_coder/utils_shim.cpp -lgtest -lgtest_main -lpthread
