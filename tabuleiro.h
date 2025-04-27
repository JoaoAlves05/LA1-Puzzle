#ifndef TABULEIRO_H
#define TABULEIRO_H

#define MAX_SIZE 100  // Tamanho máximo do tabuleiro

// Cada célula guarda o valor original (minúscula) e o estado atual (maiúscula ou '#')
typedef struct {
    char original;
    char atual;
} Celula;

// Declaração externa do tabuleiro global (definido em tabuleiro.c)
extern Celula tabuleiro[MAX_SIZE][MAX_SIZE];

// Protótipos das funções de manipulação do tabuleiro
int  input_coordenada(const char *coord,
                      int *linha, int *coluna,
                      int max_linhas, int max_colunas);
void exibirTabuleiro(int linhas, int colunas);
int  pintarDeBranco(int linha, int coluna);
int  riscarCasa(int linha, int coluna);
void gravarJogo(const char *nomeArquivo,
               int linhas, int colunas);
void carregarJogo(const char *nomeArquivo,
                 int *linhas, int *colunas);

#endif // TABULEIRO_H