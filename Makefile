# Configuration START

NAME = lox

SRC_DIR = ./src
OUT_DIR = ./bin

CFLAGS =
DEBUG_FLAGS = -Wall -Wextra -pedantic -ggdb3

# Configuration END

OUT_FILE = $(OUT_DIR)/$(NAME)
BUILD_CMD = gcc $(SRC_DIR)/*.c -o $(OUT_FILE)

build: clean
	@$(BUILD_CMD) $(CFLAGS)

debug: clean
	@$(BUILD_CMD) $(CFLAGS) $(DEBUG_FLAGS)

run: debug
	@$(OUT_FILE)

clean:
	@rm -rf $(OUT_DIR)/*
