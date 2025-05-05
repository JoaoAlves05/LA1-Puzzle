#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdio.h>

// Cada célula guarda o valor original (em minúscula) e o estado atual (maiúscula ou '#')
typedef struct {
    char original;
    char atual;
} Celula;

// Alteração: o tabuleiro agora será um ponteiro para ponteiros
extern Celula **tabuleiro; //Matriz do tipo celula(struct definida acima)
extern int tabuleiro_linhas; // Número de linhas do tabuleiro

// Protótipos das funções de manipulação do tabuleiro
void inicializarTabuleiro(int linhas, int colunas);
void liberarTabuleiro(int linhas);
int  input_coordenada(char *coord, int *linha, int *coluna,int max_linhas, int max_colunas);
void exibirTabuleiro(int linhas, int colunas);
int  pintarDeBranco(int linha, int coluna);
int  riscarCasa(int linha, int coluna);
void gravarJogo(char *nomeArquivo,int linhas, int colunas);
void carregarJogo(char *nomeArquivo,int *linhas, int *colunas);

// Protótipos das funções auxiliares para manipulação do tabuleiro
void escreverTabuleiro(FILE *arquivo, int linhas, int colunas, char tipo);
int lerTabuleiro(FILE *arquivo, int linhas, int colunas, char tipo);

#endif