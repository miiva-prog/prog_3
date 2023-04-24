APP_NAME = utf-8_function
SRC_DIR = utf-8

SRC_EXT = c

APP_SOURCES = $(shell find $(SRC_DIR) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(SRC_DIR)/%.o)
.PHONY: all
all: $(APP_NAME)

$(APP_NAME): $(APP_OBJECTS)
	gcc $^ -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	gcc -c $< -o $@