# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Iinclude


# Directories
SRC_DIR = src
INC_DIR = include

# Source Files
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/mainMenu.c $(SRC_DIR)/helper.c

# Output binary name
TARGET = SMS

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Run target (compile + execute)
run: all
	./$(TARGET)
	
# Clean build files
clean:
	-rm ${TARGET}

gpush:
	git status
	git add .
	git commit -m "😏 New Commit/Changed from Sanchit"
	git push

