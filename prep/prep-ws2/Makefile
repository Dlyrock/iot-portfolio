# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

# Paths
SRC_DIR = src
BUILD_DIR = build
LIB_DIR = /opt/iot/lib

# Include paths
INCLUDES = -Isrc
LIBS = -L$(LIB_DIR) -lsimnet

# Target executables
CLIENT_TARGET = client
SERVER_TARGET = server

# Source files
CLIENT_SRCS = $(SRC_DIR)/Client.cpp
SERVER_SRCS = $(SRC_DIR)/Server.cpp

# Object files
CLIENT_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CLIENT_SRCS))
SERVER_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SERVER_SRCS))

# Build rules
all: directories $(CLIENT_TARGET) $(SERVER_TARGET)

directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/sim

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | directories
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(BUILD_DIR) $(CLIENT_TARGET) $(SERVER_TARGET)

.PHONY: all directories clean