#ifndef TABULEIRO_H
#define TABULEIRO_H

#define MAX_SIZE 100 // Tamanho máximo do tabuleiro

typedef struct {
    char original;  // Valor inicial (minúscula nunca muda)
    char atual;     // Valor atual
} Celula;

Celula tabuleiro[MAX_SIZE][MAX_SIZE];  // Declaração do tabuleiro como uma matriz de células

void exibirTabuleiro(int linhas, int colunas);
void pintarDeBranco(int linha, int coluna);
void riscarCasa(int linha, int coluna);
void gravarJogo(char *nomeArquivo, int linhas, int colunas);
void carregarJogo(char *nomeArquivo, int *linhas, int *colunas);


#endif