# Compilador e flags
CC         = gcc
CFLAGS     = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g

# Diretórios
BUILD_DIR  = build
SRC_DIR    = src
INC_DIR    = include
TEST_DIR   = test

# Fontes da aplicação e objetos
SRCS       = $(SRC_DIR)/main.c $(SRC_DIR)/tabuleiro.c $(SRC_DIR)/historico.c $(SRC_DIR)/verificacoes.c $(SRC_DIR)/comandos.c
OBJS       = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TARGET     = jogo

# Fontes de teste e objetos de teste
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

# Compilar o jogo
jogo: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	@echo "Compilado '$@' com sucesso!"

# Compilar os testes
$(TEST_TARGETS): %: $(BUILD_DIR)/%.o $(APP_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lcunit
	@echo "Compilado teste '$@' com sucesso!"

# Regra única para compilar qualquer .c (src/ ou test/)
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

# Limpar objetos e executáveis
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGETS)
	@echo "Limpeza feita."

# Compilar e correr os testes
testar: $(TEST_TARGETS)
	@echo "A correr os testes:"
	@./test_tabuleiro
	@./test_historico
	@./test_verificacoes
