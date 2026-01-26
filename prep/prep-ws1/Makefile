# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

INCLUDES = -I/opt/iot -Isrc -Isrc/hal
LIBS = -L/opt/iot/lib -lclient 

# Source directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Target executable
TARGET = main

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SRCS))

# Test targets
TEST_TARGETS = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/test_%, $(TEST_SRCS))

# Build rules
all: build_source build_tests

build_source: $(TARGET)

build_tests: $(TEST_TARGETS)

test: build_tests
	@for t in $(TEST_TARGETS); do $$t --reporter console --verbosity normal || exit 1; done

$(TARGET): $(filter-out $(BUILD_DIR)/test_%.o, $(OBJS))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

$(BUILD_DIR)/test_%: $(BUILD_DIR)/%.o $(filter-out $(BUILD_DIR)/main.o, $(OBJS))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build files
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET) $(TEST_TARGETS)
