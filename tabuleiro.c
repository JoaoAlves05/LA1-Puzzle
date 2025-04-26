#include <stdio.h>
#include "tabuleiro.h"

void exibirTabuleiro(int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j].atual);
        }
        printf("\n");
    }
}

void pintarDeBranco(int linha, int coluna) {
    if (tabuleiro[linha][coluna].atual == tabuleiro[linha][coluna].original) {
        tabuleiro[linha][coluna].atual = toupper(tabuleiro[linha][coluna].original);
    }
}

void riscarCasa(int linha, int coluna) {
    tabuleiro[linha][coluna].atual = '#';
}

void gravarJogo(char *nomeArquivo, int linhas, int colunas) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para gravação.\n");
        return;
    }

    fprintf(arquivo, "%d %d\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "%c", tabuleiro[i][j].atual);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Jogo guardado no arquivo '%s'.\n", nomeArquivo);
}

void carregarJogo(char *nomeArquivo, int *linhas, int *colunas) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    fscanf(arquivo, "%d %d", linhas, colunas);
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            fscanf(arquivo, " %c", &tabuleiro[i][j].atual);
            tabuleiro[i][j].original = tolower(tabuleiro[i][j].atual);
        }
    }

    fclose(arquivo);
    printf("Jogo carregado do arquivo '%s'.\n", nomeArquivo);
}
