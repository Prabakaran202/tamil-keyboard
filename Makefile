CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -O3
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files and corresponding Object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Final Binary
TARGET = $(BIN_DIR)/tamizhi-keymap

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

run: all
	./$(TARGET)
