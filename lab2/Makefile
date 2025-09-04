# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/auth_system

# Source files
MAIN_SRC = $(SRC_DIR)/authentification.c
USER_SRC = $(SRC_DIR)/user.c
REGISTRATION_SRC = $(SRC_DIR)/registration.c

# Header files
USER_HEADER = $(INC_DIR)/user.h
REGISTRATION_HEADER = $(INC_DIR)/registration.h

# Object files
MAIN_OBJ = $(OBJ_DIR)/authentification.o
REGISTRATION_OBJ = $(OBJ_DIR)/registration.o
OBJS = $(MAIN_OBJ) $(REGISTRATION_OBJ)

# Default build (debug)
all: CFLAGS += $(DEBUG_FLAGS)
all: directories $(TARGET)

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: clean directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Main target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile main file
$(MAIN_OBJ): $(MAIN_SRC) $(USER_HEADER) $(REGISTRATION_HEADER)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Compile registration library
$(REGISTRATION_OBJ): $(REGISTRATION_SRC) $(REGISTRATION_HEADER)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Install (copy to /usr/local/bin)
install: release
	cp $(TARGET) /usr/local/bin/auth_system

# Uninstall
uninstall:
	rm -f /usr/local/bin/auth_system

# Run the program
run: all
	./$(TARGET) register

# Debug with gdb
debug: all
	gdb ./$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build with debug flags (default)"
	@echo "  release   - Build with optimization flags"
	@echo "  clean     - Remove build artifacts"
	@echo "  run       - Build and run the program"
	@echo "  debug     - Build and debug with gdb"
	@echo "  install   - Install to /usr/local/bin"
	@echo "  uninstall - Uninstall from /usr/local/bin"

.PHONY: all release clean install uninstall run debug help directories