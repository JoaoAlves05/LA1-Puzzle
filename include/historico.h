#ifndef HISTORICO_H
#define HISTORICO_H

#include "tabuleiro.h"

typedef struct {
    int linha, coluna;
    char valor_anterior;
    char valor_novo;
} AlteracaoTabuleiro;

typedef struct {
    AlteracaoTabuleiro *alteracoes;
    int topo;
    int capacidade;
} PilhaAlteracoes;

void inicializarPilha(PilhaAlteracoes *p);
void liberarPilha(PilhaAlteracoes *p);
void empilhar(PilhaAlteracoes *p, int linha, int coluna, char valor_anterior, char valor_novo);
int desfazer(PilhaAlteracoes *p);

#endif