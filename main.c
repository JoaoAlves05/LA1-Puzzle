#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tabuleiro.h"
#include "historico.h"
#include "verificacoes.h"

int main(void)
{
    int linhas, colunas;
    PilhaEstados historico;
    char comando[10];
    char coord[5];
    int l, c;

    // Inicializa histórico de estados
    inicializarPilha(&historico);

    // Leitura das dimensões do tabuleiro
    printf("Digite o numero de linhas e colunas do tabuleiro: ");
    if (scanf("%d %d", &linhas, &colunas) != 2 ||
        linhas < 1 || colunas < 1 ||
        linhas > MAX_SIZE || colunas > MAX_SIZE) {
        printf("Tamanho invalido (%d x %d max).\n", MAX_SIZE, MAX_SIZE);
        return 1;
    }

    // Leitura do tabuleiro inicial e empilhamento do estado base
    printf("Digite o tabuleiro inicial (%d linhas de %d caracteres):\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            char ch;
            scanf(" %c", &ch);
            tabuleiro[i][j].original = tolower(ch);
            tabuleiro[i][j].atual    = tolower(ch);
        }
    }
    // Empilha o estado inicial para possibilitar desfazer até o início
    empilhar(&historico, linhas, colunas);

    // Loop principal de comandos
    while (1) {
        printf("\nTabuleiro atual:\n");
        exibirTabuleiro(linhas, colunas);

        printf("\nComandos: b <coord>, r <coord>, d, v, g <file>, l <file>, s\n");
        printf("Digite um comando: ");
        if (scanf("%s", comando) != 1) break;

        if (strcmp(comando, "b") == 0 || strcmp(comando, "r") == 0) {
            // Leitura e validação de coordenada
            if (scanf("%4s", coord) != 1 ||
                !input_coordenada(coord, &l, &c, linhas, colunas)) {
                printf("Erro: coordenada invalida (ex: a1).\n");
                continue;
            }
            // Empilha antes de mutar
            empilhar(&historico, linhas, colunas);
            if (comando[0] == 'b') {
                if (!pintarDeBranco(l, c)) {
                    // desfaz snapshot extra se falhar ação
                    desfazerComando(&historico, &linhas, &colunas);
                }
            } else {
                if (!riscarCasa(l, c)) {
                    desfazerComando(&historico, &linhas, &colunas);
                }
            }
            // Verificação imediata após cada ação do jogador
            verificarEstado(linhas, colunas);

        } else if (strcmp(comando, "d") == 0) {
            // Desfazer último comando
            desfazerComando(&historico, &linhas, &colunas);

        } else if (strcmp(comando, "v") == 0) {
            // Verificar regras sem mutar
            verificarEstado(linhas, colunas);

        } else if (strcmp(comando, "g") == 0) {
            // Gravar estado em arquivo
            char nome[50];
            scanf("%s", nome);
            gravarJogo(nome, linhas, colunas);

        } else if (strcmp(comando, "l") == 0) {
            // Carregar estado de arquivo e reiniciar histórico
            char nome[50];
            scanf("%s", nome);
            carregarJogo(nome, &linhas, &colunas);
            inicializarPilha(&historico);
            empilhar(&historico, linhas, colunas);

        } else if (strcmp(comando, "s") == 0) {
            printf("Saindo...\n");
            break;

        } else {
            printf("Comando invalido!\n");
        }
    }

    return 0;
}