#include "tabuleiro.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>  // para atoi

// Definição do tabuleiro global
Celula tabuleiro[MAX_SIZE][MAX_SIZE];

int input_coordenada(const char *coord,
                     int *linha, int *coluna,
                     int max_linhas, int max_colunas)
{
    if (!coord || !isalpha(coord[0]) || !isdigit(coord[1]))
        return 0;

    *coluna = tolower(coord[0]) - 'a';
    *linha  = atoi(coord + 1) - 1;

    return (*linha >= 0 && *linha < max_linhas &&
            *coluna >= 0 && *coluna < max_colunas);
}

void exibirTabuleiro(int linhas, int colunas)
{
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j].atual);
        }
        printf("\n");
    }
}

int pintarDeBranco(int linha, int coluna)
{
    char atual = tabuleiro[linha][coluna].atual;
    if (atual == '#') {
        printf("Erro: Essa casa já está riscada.\n");
        return 0;
    }
    if (isupper(atual)) {
        printf("Erro: Essa casa já está pintada de branco.\n");
        return 0;
    }

    tabuleiro[linha][coluna].atual = toupper(tabuleiro[linha][coluna].original);
    return 1;
}

int riscarCasa(int linha, int coluna)
{
    char atual = tabuleiro[linha][coluna].atual;
    if (atual == '#') {
        printf("Erro: Essa casa já está riscada.\n");
        return 0;
    }
    if (isupper(atual)) {
        printf("Erro: Essa casa já está pintada de branco.\n");
        return 0;
    }

    tabuleiro[linha][coluna].atual = '#';
    return 1;
}

void gravarJogo(const char *nomeArquivo,
               int linhas, int colunas)
{
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para gravação.\n");
        return;
    }

    fprintf(arquivo, "%d %d\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fputc(tabuleiro[i][j].atual, arquivo);
        }
        fputc('\n', arquivo);
    }
    fclose(arquivo);

    printf("Jogo guardado em '%s'.\n", nomeArquivo);
}

void carregarJogo(const char *nomeArquivo,
                 int *linhas, int *colunas)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para leitura.\n");
        return;
    }

    fscanf(arquivo, "%d %d", linhas, colunas);
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            char c;
            fscanf(arquivo, " %c", &c);
            tabuleiro[i][j].atual    = c;
            tabuleiro[i][j].original = tolower(c);
        }
    }
    fclose(arquivo);

    printf("Jogo carregado de '%s'.\n", nomeArquivo);
}