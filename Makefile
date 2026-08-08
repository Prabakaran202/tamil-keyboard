# Makefile for BDH Tamizhi Keyboard Daemon (Universal Edition)

# 🔥 FIX 1: Allow system default compiler (gcc for Arch, clang for Termux)
CC ?= gcc
CFLAGS = -Wall -Wextra -Iinclude -O2
LDFLAGS = 

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# 🔥 FIX 2: Default to standard Linux path. (Termux users can override this)
PREFIX ?= /usr/local

# Find all .c files in src directory
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Output binary name
TARGET = $(BIN_DIR)/tamizhi-keymap

all: directories $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

install: $(TARGET)
	@mkdir -p $(PREFIX)/bin
	@cp $(TARGET) $(PREFIX)/bin/tamizhi-keymap
	@chmod +x $(PREFIX)/bin/tamizhi-keymap
	@echo "Installation successful! You can now run 'tamizhi-keymap' from anywhere."

.PHONY: all directories clean install
