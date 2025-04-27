#ifndef TABULEIRO_H
#define TABULEIRO_H

#define MAX_SIZE 100 // Tamanho máximo do tabuleiro

typedef struct {
    char original;  // Valor inicial (minúscula nunca muda)
    char atual;     // Valor atual
} Celula;

Celula tabuleiro[MAX_SIZE][MAX_SIZE];  // Declaração do tabuleiro como uma matriz de células

int input_coordenada(char *coordenada, int *linha, int *coluna, int max_linhas, int max_colunas);
void exibirTabuleiro(int linhas, int colunas);
int pintarDeBranco(int linha, int coluna);
int riscarCasa(int linha, int coluna);
void gravarJogo(char *nomeArquivo, int linhas, int colunas);
void carregarJogo(char *nomeArquivo, int *linhas, int *colunas);


#endif