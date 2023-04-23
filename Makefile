# IDIR =./inc
CC  = gcc
CXX = g++

SRC_DIR   = src
BUILD_DIR = build
CFLAGS    = -Wall -Wextra -g
CXXFLAGS  = -Wall -Wextra -g

LIBS =

# Define the source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SRC_FILES))) \
            $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRC_FILES)))

# Define a rule to build each object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

all: format client server

server: $(BUILD_DIR)/server.o
	$(CXX) -o $(BUILD_DIR)/$@ $^

client: $(BUILD_DIR)/client.o
	$(CXX) -o $(BUILD_DIR)/$@ $^

.PHONY: clean

clean:
	rm -f $(BUILD_DIR)/*

format:
	find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\|c\)' -exec clang-format -style=file -i {} \;
