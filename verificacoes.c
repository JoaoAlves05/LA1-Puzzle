#include <stdio.h>
#include "verificacoes.h"

void verificarEstado(char tabuleiro[MAX_SIZE][MAX_SIZE], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            char c = tabuleiro[i][j];
            if ((c >= 'a' && c <= 'z')) {
                printf("Tabuleiro incompleto. Ainda existem casas por resolver.\n");
                return;
            }
        }
    }

    printf("Tabuleiro completo! Todas as casas foram resolvidas.\n");
}
