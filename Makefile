CC       := gcc
CFLAGS   := -Wall -Wextra -pedantic -std=c11 -g -pthread
INCLUDES := -Iinclude

SRC_DIR  := src
OBJ_DIR  := build
BIN_DIR  := bin

SERVER_SRC := $(wildcard $(SRC_DIR)/server/*.c)
CLIENT_SRC := $(wildcard $(SRC_DIR)/client/*.c)
COMMON_SRC := $(wildcard $(SRC_DIR)/common/*.c)

SERVER_SRC += $(COMMON_SRC)
CLIENT_SRC += $(COMMON_SRC)

SERVER_OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SERVER_SRC))
CLIENT_OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CLIENT_SRC))

DEPFLAGS := -MMD -MP
CFLAGS  += $(DEPFLAGS)

SERVER_BIN := $(BIN_DIR)/server
CLIENT_BIN := $(BIN_DIR)/client

.PHONY: all server client clean
all: $(SERVER_BIN) $(CLIENT_BIN)

server: $(SERVER_BIN)
client: $(CLIENT_BIN)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(SERVER_BIN): $(SERVER_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(CLIENT_BIN): $(CLIENT_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

-include $(SERVER_OBJ:.o=.d) $(CLIENT_OBJ:.o=.d)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
