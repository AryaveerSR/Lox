# Configuration START

NAME = lox

SRC_DIR = ./src
OUT_DIR = ./bin
TEST_DIR = ./tests
INCLUDE_DIR = ./include

CFLAGS =
DEBUG_FLAGS = -Wall -Wextra -pedantic -ggdb3

# Configuration END

OUT_FILE = $(OUT_DIR)/$(NAME)
BUILD_CMD = gcc $(SRC_DIR)/*.c -I$(INCLUDE_DIR) -o $(OUT_FILE)

# Tests related variables

TEST_OUT_FILE = $(OUT_DIR)/test_$(NAME)

TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_SRC_FILES = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))

TEST_BUILD_CMD = gcc $(TEST_SRC_FILES) -I$(INCLUDE_DIR) -o $(TEST_OUT_FILE)

build: clean
	@$(BUILD_CMD) $(CFLAGS)

debug: clean
	@$(BUILD_CMD) $(CFLAGS) $(DEBUG_FLAGS)

run: debug
	@$(OUT_FILE)

test: clean
	@for test in $(TEST_FILES); do \
		$(TEST_BUILD_CMD) $$test $(CFLAGS); \
		echo $$test; \
		$(TEST_OUT_FILE); \
		echo; \
		rm -rf $(OUT_DIR)/*; \
	done

clean:
	@rm -rf $(OUT_DIR)/*
