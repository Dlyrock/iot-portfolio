# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

# Paths
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
TEST_BUILD_DIR = $(BUILD_DIR)/tests
LIB_DIR = /opt/iot/lib
INCLUDE_DIR = /opt/iot/include
POCO_DIR = /opt/iot/poco
CATCH2_DIR = /opt/iot

# Server library paths
INCLUDES = -I$(POCO_DIR)/include -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/web -I$(INCLUDE_DIR)/util -I$(SRC_DIR)
LIBS = -L$(LIB_DIR) -lwebserver -L$(POCO_DIR)/lib -lPocoNet -lPocoUtil -lPocoFoundation -lPocoJSON -lpthread
LDFLAGS = -Wl,-rpath,$(POCO_DIR)/lib

# Clients library paths
CL_INCLUDES = -I$(INCLUDE_DIR)
CLILIBS = -L$(LIB_DIR) -lclient -L$(POCO_DIR)/lib -lPocoNet -lPocoUtil -lPocoFoundation -lPocoJSON -lpthread
CL_LDFLAGS = -Wl,-rpath,$(POCO_DIR)/lib

# Test paths
TEST_INCLUDES = -I$(CATCH2_DIR) $(INCLUDES) $(CL_INCLUDES)
TEST_LIBS = $(LIBS) -lclient -L$(POCO_DIR)/lib -lPocoNet -lPocoUtil -lPocoFoundation -lPocoJSON -lpthread

# Target executables
# DO NOT CHANGE THE NAMES OF THE TARGETS
EBIKE_CLIENT_TARGET = ebikeClient



# Source files
EBIKE_CLIENT_SRCS = $(wildcard $(SRC_DIR)/ebikeClient.cpp)

# Test source files
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)

# Test executables (one per test source file)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp, $(TEST_BUILD_DIR)/%, $(TEST_SRCS))

# Object files
EBIKE_CLIENT_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(EBIKE_CLIENT_SRCS))

# Test object files
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp, $(TEST_BUILD_DIR)/%.o, $(TEST_SRCS))

# Build rules
all: $(BUILD_DIR) $(EBIKE_CLIENT_TARGET) tests

tests: $(TEST_BINS)

# Run all tests
test: tests
	@for t in $(TEST_BINS); do $$t --reporter console --verbosity normal || exit 1; done

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(TEST_BUILD_DIR)

$(EBIKE_CLIENT_TARGET): $(EBIKE_CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) $(CL_INCLUDES) -o $@ $^ $(CLILIBS) $(CL_LDFLAGS)

# Pattern rule to build each main object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Pattern rule to build each test object file
$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) -c $< -o $@

# Pattern rule to build each test executable
$(TEST_BUILD_DIR)/%: $(TEST_BUILD_DIR)/%.o $(LIBRARY_CL)
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) -o $@ $^ $(TEST_LIBS) $(LDFLAGS)

# Run a specific test with arguments: make run-test TEST=name ARGS="--list-tests"
run-test: tests
	@$(TEST_BUILD_DIR)/$(TEST) $(ARGS)

run-ebike-client:
	@./$(EBIKE_CLIENT_TARGET) $(ARGS)

# Clean up build files
clean:
	rm -rf $(BUILD_DIR) $(EBIKE_CLIENT_TARGET) $(TEST_BUILD_DIR)/*