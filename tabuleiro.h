#ifndef TABULEIRO_H
#define TABULEIRO_H

#define MAX_SIZE 100

void exibirTabuleiro(char tabuleiro[MAX_SIZE][MAX_SIZE], int linhas, int colunas);
void pintarDeBranco(char tabuleiro[MAX_SIZE][MAX_SIZE], int linha, int coluna);
void riscarCasa(char tabuleiro[MAX_SIZE][MAX_SIZE], int linha, int coluna);
void gravarJogo(char *nomeArquivo, char tabuleiro[MAX_SIZE][MAX_SIZE], int linhas, int colunas);
void carregarJogo(char *nomeArquivo, char tabuleiro[MAX_SIZE][MAX_SIZE], int *linhas, int *colunas);

#endif // TABULEIRO_H