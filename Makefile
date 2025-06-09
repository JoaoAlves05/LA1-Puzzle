# Compilador e flags de compilação
CC         = gcc
CFLAGS     = -Wall -Wextra -pedantic -std=c99 -g

# Diretórios do projeto
BUILD_DIR  = build
SRC_DIR    = src
INC_DIR    = include
TEST_DIR   = test

TEST_SRCS    = $(wildcard $(TEST_DIR)/*.c)
TEST_BINS    = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%, $(TEST_SRCS))

# Ficheiros da aplicação principal
SRCS       = $(SRC_DIR)/main.c $(SRC_DIR)/tabuleiro.c $(SRC_DIR)/historico.c $(SRC_DIR)/verificacoes.c $(SRC_DIR)/comandos.c
OBJS       = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TARGET     = puzzle

INCLUDES = -I$(INC_DIR)

# Alvos que não são ficheiros
.PHONY: all clean tests build testar puzzle jogo

# Compilar o executável principal do jogo
all: puzzle testar

# Alvo principal para compilar o executável do jogo
puzzle: build
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SRCS)
	@echo "Compilado 'puzzle' com sucesso!"

# Alias antigo para compatibilidade
jogo: puzzle

# Alias para testar
tests: testar

testar: build $(TEST_BINS)
	@echo "Testes compilados em $(BUILD_DIR)/"

# Compila cada teste individualmente, ligando sempre com todos os módulos necessários
$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(SRC_DIR)/tabuleiro.c $(SRC_DIR)/historico.c $(SRC_DIR)/verificacoes.c $(SRC_DIR)/comandos.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ -lcunit

build:
	mkdir -p $(BUILD_DIR)

# Limpar todos os objetos e executáveis gerados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Limpeza feita."
