# Compiler and flags
CXX := g++
CXX_FLAGS := -std=c++20 -Wall -Wextra -c

# project info
SRC_DIR := src
BUILD_DIR := build

TARGET := hex-dump

SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR)/%.o, $(SRC_FILES))

all: setup $(TARGET)

setup:
	mkdir $(BUILD_DIR)

%.o : %.cpp
	$(CXX) $(CXX_FLAGS) -o $@ $<

$(TARGET): $(OBJ_FILES)	
	mv $(OBJ_FILES) $(BUILD_DIR)
	$(CXX) -o $@ ./$(BUILD_DIR)/*.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) || true
	rm $(TARGET) || true

.PHONY: rb
rb: clean all
