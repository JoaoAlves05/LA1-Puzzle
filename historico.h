#ifndef HISTORICO_H
#define HISTORICO_H

#include "tabuleiro.h"

typedef struct {
    char estado[MAX_SIZE][MAX_SIZE];
    int linhas;
    int colunas;
} EstadoTabuleiro;

typedef struct {
    EstadoTabuleiro pilha[100];
    int topo;
} PilhaEstados;

void inicializarPilha(PilhaEstados *p);
int empilhar(PilhaEstados *p, char tabuleiro[MAX_SIZE][MAX_SIZE], int linhas, int colunas);
int desempilhar(PilhaEstados *p, char tabuleiro[MAX_SIZE][MAX_SIZE], int *linhas, int *colunas);
void desfazerComando(PilhaEstados *historico, char tabuleiro[MAX_SIZE][MAX_SIZE], int *linhas, int *colunas);

#endif // HISTORICO_H