#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tabuleiro.h"
#include "historico.h"
#include "verificacoes.h"

int main() {
    int linhas, colunas;
    PilhaEstados historico;
    Celula tabuleiro[MAX_SIZE][MAX_SIZE];

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
            char c;
            scanf(" %c", &c);
            tabuleiro[i][j].original = tolower(c);  // Armazena o valor original
            tabuleiro[i][j].atual = tolower(c);     // Armazena o valor atual(= ao original no inicio)
        }
    }

    // Loop principal do jogo
    char comando[10];
    char coordenadas[5];
    int linha, coluna;

    while (1) {
        printf("\nTabuleiro atual:\n");
        exibirTabuleiro(linhas, colunas);

        printf("\nComandos disponiveis:\n");
        printf("b <linha><coluna> - Pintar de branco\n");
        printf("r <linha><coluna> - Riscar casa\n");
        printf("g <arquivo> - Gravar jogo\n");
        printf("l <arquivo> - Carregar jogo\n");
        printf("d - Desfazer Comando\n");
        printf("v - Verificar estado do tabuleiro\n");
        printf("s - Sair\n");
        printf("Digite um comando: ");
        scanf("%s", comando);

        if (strcmp(comando, "b") == 0 || strcmp(comando, "r") == 0) {
            // Verifica se a coordenada foi fornecida corretamente
            if (scanf("%4s", coordenadas) != 1) {
                printf("Erro: Digite uma coordenada (ex: 'b a1' ou 'r b3').\n");
                while (getchar() != '\n'); // Limpa buffer de entrada
                continue;
            }
            if (!input_coordenada(coordenadas, &linha, &coluna, linhas, colunas)) {
                printf("Erro: Coordenada '%s' inválida. Use <letra><número> (ex: 'a1').\n", coordenadas);
                continue;
            }

            // Empilha o tabuleiro apenas se as ações forem válidas
            if (strcmp(comando, "b") == 0) {
                if (pintarDeBranco(linha, coluna)) {
                    empilhar(&historico, linhas, colunas);
                }
            } else {
                if (riscarCasa(linha, coluna)) {
                    empilhar(&historico, linhas, colunas);
                }
            }

        } else if (strncmp(comando, "g", 1) == 0) {
            char nomeArquivo[50];
            scanf("%s", nomeArquivo);
            gravarJogo(nomeArquivo, linhas, colunas);

        } else if (strncmp(comando, "l", 1) == 0) {
            char nomeArquivo[50];
            scanf("%s", nomeArquivo);
            carregarJogo(nomeArquivo, &linhas, &colunas);

        } else if (strncmp(comando, "d", 1) == 0) {
            desfazerComando(&historico, &linhas, &colunas);

        } else if (strncmp(comando, "v", 1) == 0) {
            verificarEstado(linhas, colunas);

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