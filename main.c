#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"
#include "historico.h"
#include "verificacoes.h"

int main() {
    int linhas, colunas;
    char tabuleiro[MAX_SIZE][MAX_SIZE];
    PilhaEstados historico;

    inicializarPilha(&historico);

    // Leitura do tamanho do tabuleiro
    printf("Digite o numero de linhas e colunas do tabuleiro: ");
    if (scanf("%d %d", &linhas, &colunas) != 2 || linhas > MAX_SIZE || colunas > MAX_SIZE) {
        printf("Tamanho do tabuleiro invalido ou excede o maximo permitido (%d x %d).\n", MAX_SIZE, MAX_SIZE);
        return 1;
    }

    // Leitura do tabuleiro inicial
    printf("Digite o tabuleiro inicial (%d linhas de %d caracteres):\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            scanf(" %c", &tabuleiro[i][j]);
        }
    }

    // Loop principal do jogo
    char comando[10];
    int linha, coluna;
    while (1) {
        printf("\nTabuleiro atual:\n");
        exibirTabuleiro(tabuleiro, linhas, colunas);

        printf("\nComandos disponiveis:\n");
        printf("b <linha> <coluna> - Pintar de branco\n");
        printf("r <linha> <coluna> - Riscar casa\n");
        printf("g <arquivo> - Gravar jogo\n");
        printf("l <arquivo> - Carregar jogo\n");
        printf("d - Desfazer Comando\n");
        printf("v - Verificar estado do tabuleiro\n");
        printf("s - Sair\n");
        printf("Digite um comando: ");
        scanf("%s", comando);

        if (strncmp(comando, "b", 1) == 0) {
            if (scanf("%d %d", &linha, &coluna) != 2 || linha < 1 || linha > linhas || coluna < 1 || coluna > colunas) {
                printf("Coordenadas invalidas!\n");
                while (getchar() != '\n');
                continue;
            }
            empilhar(&historico, tabuleiro, linhas, colunas);
            pintarDeBranco(tabuleiro, linha - 1, coluna - 1);

        } else if (strncmp(comando, "r", 1) == 0) {
            if (scanf("%d %d", &linha, &coluna) != 2 || linha < 1 || linha > linhas || coluna < 1 || coluna > colunas) {
                printf("Coordenadas invalidas!\n");
                while (getchar() != '\n');
                continue;
            }
            empilhar(&historico, tabuleiro, linhas, colunas);
            riscarCasa(tabuleiro, linha - 1, coluna - 1);

        } else if (strncmp(comando, "g", 1) == 0) {
            char nomeArquivo[50];
            scanf("%s", nomeArquivo);
            gravarJogo(nomeArquivo, tabuleiro, linhas, colunas);

        } else if (strncmp(comando, "l", 1) == 0) {
            char nomeArquivo[50];
            scanf("%s", nomeArquivo);
            carregarJogo(nomeArquivo, tabuleiro, &linhas, &colunas);

        } else if (strncmp(comando, "d", 1) == 0) {
            desfazerComando(&historico, tabuleiro, &linhas, &colunas);

        } else if (strncmp(comando, "v", 1) == 0) {
            verificarEstado(tabuleiro, linhas, colunas);

        } else if (strncmp(comando, "s", 1) == 0) {
            printf("Saindo do jogo...\n");
            break;

        } else {
            printf("Comando invalido!\n");
            while (getchar() != '\n');
        }
    }

    return 0;
}