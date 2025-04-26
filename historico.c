#include <stdio.h>
#include <string.h>
#include "historico.h"

void inicializarPilha(PilhaEstados *p) {
    p->topo = -1;
}

int empilhar(PilhaEstados *p, int linhas, int colunas) {
    if (p->topo >= 99) return 0; // Verifica se a pilha está cheia

    p->topo++;
    p->pilha[p->topo].linhas = linhas;
    p->pilha[p->topo].colunas = colunas;

    // Copia o estado atual do tabuleiro para a pilha
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            p->pilha[p->topo].celulas[i][j] = tabuleiro[i][j];
        }
    }
    return 1;
}

// Restaura o estado anterior do tabuleiro da pilha
int desempilhar(PilhaEstados *p, int *linhas, int *colunas) {
    if (p->topo < 0) return 0;

    *linhas = p->pilha[p->topo].linhas;
    *colunas = p->pilha[p->topo].colunas;

    // Copia o estado do topo da pilha para o tabuleiro
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            tabuleiro[i][j] = p->pilha[p->topo].celulas[i][j];
        }
    }
    p->topo--;
    return 1;
}

// Desfaz o último comando e portanto restaura o estado anterior do tabuleiro
void desfazerComando(PilhaEstados *historico, int *linhas, int *colunas) {
    if (desempilhar(historico, linhas, colunas)) {
        printf("Comando desfeito.\n");
    } else {
        printf("Nenhum comando para desfazer.\n");
    }
}