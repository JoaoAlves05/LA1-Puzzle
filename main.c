#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tabuleiro.h"
#include "historico.h"
#include "verificacoes.h"

int main(void) {
    Celula tabuleiro[MAX_SIZE][MAX_SIZE]; // Declare the board locally
    int linhas, colunas;
    PilhaEstados historico;
    char comando[10];
    char coord[5];
    int l, c;

    // Initialize the history stack
    inicializarPilha(&historico);

    // Read board dimensions
    printf("Digite o numero de linhas e colunas do tabuleiro: ");
    if (scanf("%d %d", &linhas, &colunas) != 2 ||
        linhas < 1 || colunas < 1 ||
        linhas > MAX_SIZE || colunas > MAX_SIZE) {
        printf("Tamanho invalido (%d x %d max).\n", MAX_SIZE, MAX_SIZE);
        return 1;
    }

    // Read the initial board and push the initial state
    printf("Digite o tabuleiro inicial (%d linhas de %d caracteres):\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            char ch;
            if (scanf(" %c", &ch) != 1) {
                printf("Erro de leitura de caractere.\n");
                return 1;
            }
            tabuleiro[i][j].original = tolower(ch);
            tabuleiro[i][j].atual = tolower(ch);
        }
    }
    empilhar(&historico, tabuleiro, linhas, colunas);

    // Main command loop
    while (1) {
        printf("\nTabuleiro atual:\n");
        exibirTabuleiro(tabuleiro, linhas, colunas);

        printf("\nComandos: b <coord>, r <coord>, d, v, g <file>, l <file>, s\n");
        printf("Digite um comando: ");
        if (scanf("%s", comando) != 1) break;

        if (strcmp(comando, "b") == 0 || strcmp(comando, "r") == 0) {
            if (scanf("%4s", coord) != 1 ||
                !input_coordenada(coord, &l, &c, linhas, colunas)) {
                printf("Erro: coordenada invalida (ex: a1).\n");
                continue;
            }
            empilhar(&historico, tabuleiro, linhas, colunas);
            if (comando[0] == 'b') {
                if (!pintarDeBranco(tabuleiro, l, c)) {
                    desfazerComando(&historico, tabuleiro, &linhas, &colunas);
                }
            } else {
                if (!riscarCasa(tabuleiro, l, c)) {
                    desfazerComando(&historico, tabuleiro, &linhas, &colunas);
                }
            }
            verificarEstado(tabuleiro, linhas, colunas);

        } else if (strcmp(comando, "d") == 0) {
            desfazerComando(&historico, tabuleiro, &linhas, &colunas);

        } else if (strcmp(comando, "v") == 0) {
            verificarEstado(tabuleiro, linhas, colunas);

        } else if (strcmp(comando, "g") == 0) {
            char nome[50];
            if (scanf("%49s", nome) != 1) {
                printf("Erro de leitura de nome de arquivo.\n");
                return 1;
            }
            gravarJogo(tabuleiro, nome, linhas, colunas);

        } else if (strcmp(comando, "l") == 0) {
            char nome[50];
            if (scanf("%49s", nome) != 1) {
                printf("Erro de leitura de nome de arquivo.\n");
                return 1;
            }
            carregarJogo(tabuleiro, nome, &linhas, &colunas);
            inicializarPilha(&historico);
            empilhar(&historico, tabuleiro, linhas, colunas);

        } else if (strcmp(comando, "R") == 0) {
            printf("Resolver jogo:\n");
            resolver(tabuleiro, linhas, colunas);

        } else if (strcmp(comando, "s") == 0) {
            printf("Saindo...\n");
            break;

        } else {
            printf("Comando invalido!\n");
        }
    }

    return 0;
}