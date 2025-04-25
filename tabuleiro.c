#include <stdio.h>
#include "tabuleiro.h"

void exibirTabuleiro(char tabuleiro[MAX_SIZE][MAX_SIZE], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void pintarDeBranco(char tabuleiro[MAX_SIZE][MAX_SIZE], int linha, int coluna) {
    if (tabuleiro[linha][coluna] >= 'a' && tabuleiro[linha][coluna] <= 'z') {
        tabuleiro[linha][coluna] -= 32; // Converte para maiúscula
    }
}

void riscarCasa(char tabuleiro[MAX_SIZE][MAX_SIZE], int linha, int coluna) {
    tabuleiro[linha][coluna] = '#';
}

void gravarJogo(char *nomeArquivo, char tabuleiro[MAX_SIZE][MAX_SIZE], int linhas, int colunas) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para gravação.\n");
        return;
    }

    fprintf(arquivo, "%d %d\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "%c", tabuleiro[i][j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Jogo salvo no arquivo '%s'.\n", nomeArquivo);
}

void carregarJogo(char *nomeArquivo, char tabuleiro[MAX_SIZE][MAX_SIZE], int *linhas, int *colunas) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    fscanf(arquivo, "%d %d", linhas, colunas);
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            fscanf(arquivo, " %c", &tabuleiro[i][j]);
        }
    }

    fclose(arquivo);
    printf("Jogo carregado do arquivo '%s'.\n", nomeArquivo);
}
