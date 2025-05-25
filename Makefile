CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Iinclude/common -Iinclude/server -Iinclude/client -pthread
BUILD_DIR = build

SRC_DIR = src
CLIENT_SRC = $(SRC_DIR)/client/main.c $(SRC_DIR)/common/common_setup.c
SERVER_SRC = $(SRC_DIR)/server/main.c $(SRC_DIR)/server/connection.c \
             $(SRC_DIR)/server/formatter.c $(SRC_DIR)/server/server.c \
             $(SRC_DIR)/server/server_setup.c $(SRC_DIR)/common/common_setup.c

CLIENT_BIN = $(BUILD_DIR)/client
SERVER_BIN = $(BUILD_DIR)/server

all: $(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_BIN): $(CLIENT_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_BIN): $(SERVER_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

