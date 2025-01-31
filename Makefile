NAME = flappy-with-items
VERSION = 1.0.0

CC = cc
CFLAGS = -Wall -g
LDFLAGS = -lncurses

SRC_DIR = src
OBJ_DIR = target/obj
BIN_DIR = target/bin
DIST_DIR = target/dist/$(NAME)-$(VERSION)
DIST_TGZ = target/dist/$(NAME)-$(VERSION).tgz

TARGET = $(BIN_DIR)/$(NAME)

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

clean-all:
	rm -rf target

dist: $(TARGET)
	@echo "Creating distribution directory: $(DIST_DIR)..."
	@mkdir -p $(DIST_DIR)/bin
	@cp -r $(SRC_DIR) $(DIST_DIR)/
	@cp $(TARGET) $(DIST_DIR)/bin/
	@cp Makefile $(DIST_DIR)/

	@echo "Compressing to $(DIST_TGZ)..."
	@tar -czf $(DIST_TGZ) -C target/dist $(NAME)-$(VERSION)

	@echo "Cleaning up temporary directory..."
	@rm -rf $(DIST_DIR)

	@echo "Distribution package created: $(DIST_TGZ)"

.PHONY: all clean clean-all dist run
