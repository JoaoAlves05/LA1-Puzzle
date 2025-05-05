#ifndef TABULEIRO_H
#define TABULEIRO_H

#include "celula.h" // Include the Celula structure definition

#define MAX_SIZE 100 // Define the maximum size of the board

void exibirTabuleiro(Celula (*tabuleiro)[MAX_SIZE], int linhas, int colunas);
int input_coordenada(char *coord, int *linha, int *coluna, int max_linhas, int max_colunas);
int pintarDeBranco(Celula (*tabuleiro)[MAX_SIZE], int linha, int coluna);
int riscarCasa(Celula (*tabuleiro)[MAX_SIZE], int linha, int coluna);
void gravarJogo(Celula (*tabuleiro)[MAX_SIZE], char *nomeArquivo, int linhas, int colunas);
void carregarJogo(Celula (*tabuleiro)[MAX_SIZE], char *nomeArquivo, int *linhas, int *colunas);
void resolver(Celula (*tabuleiro)[MAX_SIZE], int linhas, int colunas);

#endif