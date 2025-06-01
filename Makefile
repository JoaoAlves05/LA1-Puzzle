# Compilador e flags de compilação
CC         = gcc
CFLAGS     = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g

# Diretórios do projeto
BUILD_DIR  = build
SRC_DIR    = src
INC_DIR    = include
TEST_DIR   = test

# Ficheiros da aplicação principal
SRCS       = $(SRC_DIR)/main.c $(SRC_DIR)/tabuleiro.c $(SRC_DIR)/historico.c $(SRC_DIR)/verificacoes.c $(SRC_DIR)/comandos.c
OBJS       = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TARGET     = jogo

# Ficheiros e objetos dos testes
TEST_SRCS    = $(TEST_DIR)/test_tabuleiro.c $(TEST_DIR)/test_historico.c $(TEST_DIR)/test_verificacoes.c
TEST_OBJS    = $(TEST_SRCS:$(TEST_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_TARGETS = test_tabuleiro test_historico test_verificacoes

# Objetos da aplicação usados pelos testes
APP_OBJS   = $(BUILD_DIR)/tabuleiro.o \
             $(BUILD_DIR)/historico.o \
             $(BUILD_DIR)/verificacoes.o \
             $(BUILD_DIR)/comandos.o

# Alvos que não são ficheiros
.PHONY: clean testar jogo

# Compilar o executável principal do jogo
jogo: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	@echo "Compilado '$@' com sucesso!"

# Compilar os testes unitários
$(TEST_TARGETS): %: $(BUILD_DIR)/%.o $(APP_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lcunit
	@echo "Compilado teste '$@' com sucesso!"

# Regra para compilar qualquer ficheiro .c (da src/ ou test/)
$(BUILD_DIR)/%.o:
	mkdir -p $(BUILD_DIR)
	@if [ -f $(SRC_DIR)/$*.c ]; then \
		$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/$*.c -o $@; \
	elif [ -f $(TEST_DIR)/$*.c ]; then \
		$(CC) $(CFLAGS) -I$(INC_DIR) -c $(TEST_DIR)/$*.c -o $@; \
	else \
		echo "Erro: Fonte $*.c não encontrada em $(SRC_DIR) ou $(TEST_DIR)"; \
		exit 1; \
	fi

# Limpar todos os objetos e executáveis gerados
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGETS)
	@echo "Limpeza feita."

# Compilar e correr todos os testes
testar: $(TEST_TARGETS)
	@echo "A correr os testes:"
	@./test_tabuleiro
	@./test_historico
	@./test_verificacoes
