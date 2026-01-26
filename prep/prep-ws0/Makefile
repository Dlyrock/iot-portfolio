# 1. Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# 2. Directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# 3. Targets
MAIN_EXEC = $(BUILD_DIR)/main
TEST_EXEC = $(BUILD_DIR)/test_add

# 4. Source files
ADD_SRC = $(SRC_DIR)/add.cpp
ADD_HDR = $(SRC_DIR)/add.h

# 5. Default target: build everything
all: $(MAIN_EXEC) $(TEST_EXEC)

# 6. Build main executable
$(MAIN_EXEC): $(SRC_DIR)/main.cpp $(ADD_SRC) $(ADD_HDR)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_DIR)/main.cpp $(ADD_SRC)

# 7. Build test executable
$(TEST_EXEC): $(TEST_DIR)/test_add.cpp $(ADD_SRC) $(ADD_HDR)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_DIR)/test_add.cpp $(ADD_SRC)

# 8. Run tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# 9. Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test clean