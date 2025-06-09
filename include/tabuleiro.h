#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdio.h>

// Estrutura que representa uma célula do tabuleiro. original guarda o valor inicial (minúscula), atual o estado atual (maiúscula ou '#')
typedef struct {
    char original;
    char atual;
} Celula;

// Matriz global do tabuleiro (ponteiro para ponteiros de Celula)
extern Celula **tabuleiro;
extern int tabuleiro_linhas; // Número de linhas do tabuleiro

// Inicializa o tabuleiro com as dimensões dadas.
void inicializarTabuleiro(int linhas, int colunas);

// Liberta a memória do tabuleiro.
void liberarTabuleiro(int linhas);

// Converte uma string de coordenada (ex: "a1") para índices de linha e coluna.
// Retorna 1 se válido, 0 caso contrário.
int  input_coordenada(char *coord, int *linha, int *coluna,int max_linhas, int max_colunas);

// Mostra o tabuleiro no ecrã.
void exibirTabuleiro(int linhas, int colunas);

// Pinta uma casa de branco (maiúscula). Retorna 1 se conseguiu, 0 caso contrário.
int  pintarDeBranco(int linha, int coluna);

// Risca uma casa ('#'). Retorna 1 se conseguiu, 0 caso contrário.
int  riscarCasa(int linha, int coluna);

// Grava o estado do jogo num ficheiro.
void gravarJogo(char *nomeArquivo,int linhas, int colunas);

// Carrega o estado do jogo de um ficheiro.
void carregarJogo(char *nomeArquivo,int *linhas, int *colunas);

// Escreve o tabuleiro num ficheiro (auxiliar).
void escreverTabuleiro(FILE *arquivo, int linhas, int colunas, char tipo);

// Lê o tabuleiro de um ficheiro (auxiliar).
int lerTabuleiro(FILE *arquivo, int linhas, int colunas, char tipo);

// Macros para cores ANSI
#define ANSI_RESET   "\033[0m"
#define ANSI_RED     "\033[31m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_CYAN    "\033[36m"
#define ANSI_BOLD    "\033[1m"

// Funções auxiliares de interface
void clear_screen(void);
void pause_enter(const char *msg);
int ler_linha(char *buffer, int max);
void ascii_art(void);

#endif