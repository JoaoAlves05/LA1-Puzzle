#include <stdio.h>
#include "historico.h"

void inicializarPilha(PilhaEstados *p)
{
    p->topo = -1;
}

int empilhar(PilhaEstados *p, int linhas, int colunas)
{
    if (p->topo >= MAX_HISTORICO - 1)
        return 0;

    p->topo++;
    EstadoTabuleiro *e = &p->pilha[p->topo];
    e->linhas  = linhas;
    e->colunas = colunas;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            e->celulas[i][j] = tabuleiro[i][j];
        }
    }
    return 1;
}

int desempilhar(PilhaEstados *p, int *linhas, int *colunas)
{
    if (p->topo < 0)
        return 0;

    EstadoTabuleiro *e = &p->pilha[p->topo--];
    *linhas  = e->linhas;
    *colunas = e->colunas;
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            tabuleiro[i][j] = e->celulas[i][j];
        }
    }
    return 1;
}

void desfazerComando(PilhaEstados *p, int *linhas, int *colunas)
{
    if (desempilhar(p, linhas, colunas))
        printf("Comando desfeito.\n");
    else
        printf("Nenhum comando para desfazer.\n");
}