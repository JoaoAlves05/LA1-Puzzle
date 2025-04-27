CC         = gcc
CFLAGS     = -Wall -Wextra -pedantic -O1 -fno-omit-frame-pointer -g
BUILD_DIR  = objetos

# Fontes da aplicação e objetos
SRCS       = main.c tabuleiro.c historico.c verificacoes.c
OBJS       = $(SRCS:%.c=$(BUILD_DIR)/%.o)
TARGET     = jogo

# Fontes de teste e executáveis de teste
TEST_SRCS    = test_tabuleiro.c test_historico.c test_verificacoes.c
TEST_OBJS    = $(TEST_SRCS:%.c=$(BUILD_DIR)/%.o)
TEST_TARGETS = test_tabuleiro test_historico test_verificacoes

# Objetos da aplicação (sem main.o) usados pelos testes
APP_OBJS   = $(BUILD_DIR)/tabuleiro.o \
             $(BUILD_DIR)/historico.o \
             $(BUILD_DIR)/verificacoes.o

# Alvo padrão: compila o binário principal
all: $(TARGET)

# Gera o executável principal
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Gera cada executável de teste (usa só APP_OBJS + test .o, não inclui main.o)
$(TEST_TARGETS): %: $(BUILD_DIR)/%.o $(APP_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lcunit

# Regra genérica para compilar .c em .o
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean testar

# Limpa executáveis e objetos
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGETS)

# Compila e executa os testes
testar: $(TEST_TARGETS)
	@echo "Running CUnit tests..."
	@./test_tabuleiro
	@./test_historico
	@./test_verificacoes
