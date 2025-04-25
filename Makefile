CC = gcc
CFLAGS = -Wall -Wextra -g
BUILD_DIR = build

SRCS = main.c tabuleiro.c historico.c verificacoes.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
TARGET = puzzle

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
