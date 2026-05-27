# Compiler and flags
CXX = g++
CXXFLAGS = -pedantic-errors -std=c++11
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O3

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Target executable
TARGET = $(BIN_DIR)/pacman_game

# Default target
all: $(TARGET)

# Create directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Link object files to create executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Release build
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the game
run: $(TARGET)
	./$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all     - Build the game (default)"
	@echo "  debug   - Build with debug flags"
	@echo "  release - Build with optimization flags"
	@echo "  clean   - Remove all build artifacts"
	@echo "  run     - Build and run the game"
	@echo "  help    - Show this help message"

# Phony targets
.PHONY: all debug release clean run help

# Dependency information
$(BUILD_DIR)/main.o: $(INCLUDE_DIR)/GameEngine.h
$(BUILD_DIR)/GameEngine.o: $(INCLUDE_DIR)/GameEngine.h $(INCLUDE_DIR)/common.h $(INCLUDE_DIR)/map.h $(INCLUDE_DIR)/player.h $(INCLUDE_DIR)/inputhandler.h $(INCLUDE_DIR)/toxin.h $(INCLUDE_DIR)/topscore.h
$(BUILD_DIR)/player.o: $(INCLUDE_DIR)/player.h $(INCLUDE_DIR)/common.h $(INCLUDE_DIR)/component.h $(INCLUDE_DIR)/map.h
$(BUILD_DIR)/map.o: $(INCLUDE_DIR)/map.h $(INCLUDE_DIR)/common.h $(INCLUDE_DIR)/wall.h $(INCLUDE_DIR)/cookie.h $(INCLUDE_DIR)/player.h $(INCLUDE_DIR)/toxin.h
$(BUILD_DIR)/inputhandler.o: $(INCLUDE_DIR)/inputhandler.h $(INCLUDE_DIR)/player.h $(INCLUDE_DIR)/map.h
$(BUILD_DIR)/toxin.o: $(INCLUDE_DIR)/toxin.h $(INCLUDE_DIR)/common.h $(INCLUDE_DIR)/map.h $(INCLUDE_DIR)/component.h
$(BUILD_DIR)/cookie.o: $(INCLUDE_DIR)/cookie.h $(INCLUDE_DIR)/component.h
$(BUILD_DIR)/level.o: $(INCLUDE_DIR)/level.h
$(BUILD_DIR)/topscore.o: $(INCLUDE_DIR)/topscore.h
$(BUILD_DIR)/wall.o: $(INCLUDE_DIR)/wall.h $(INCLUDE_DIR)/component.h