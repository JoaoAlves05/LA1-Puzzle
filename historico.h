#ifndef HISTORICO_H
#define HISTORICO_H

#include "tabuleiro.h"

#define MAX_HISTORICO 100

typedef struct {
    Celula celulas[MAX_SIZE][MAX_SIZE];
    int linhas;
    int colunas;
} EstadoTabuleiro;

typedef struct {
    EstadoTabuleiro pilha[100];
    int topo;
} PilhaEstados;

void inicializarPilha(PilhaEstados *p);
int empilhar(PilhaEstados *p, int linhas, int colunas);               
int desempilhar(PilhaEstados *p, int *linhas, int *colunas);
void desfazerComando(PilhaEstados *historico, int *linhas, int *colunas);

#endif // HISTORICO_H