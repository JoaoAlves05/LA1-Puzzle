#include "tabuleiro.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>  // para atoi
#include"verificacoes.h"
#include "historico.h"
#include <string.h>
#include "verificacoes.h"
// Definição do tabuleiro global
Celula tabuleiro[MAX_SIZE][MAX_SIZE];

int input_coordenada(char *coord,
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

void exibirTabuleiro(Celula (*tabuleiro)[MAX_SIZE], int linhas, int colunas) {
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

void gravarJogo(char *nomeArquivo,
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

void carregarJogo(char *nomeArquivo,
                 int *linhas, int *colunas)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para leitura.\n");
        return;
    }

    if (fscanf(arquivo, "%d %d", linhas, colunas) != 2) {
        printf("Erro no formato do arquivo: linhas/colunas.\n");
        fclose(arquivo);
        return;
    }
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            char c;
            if (fscanf(arquivo, " %c", &c) != 1) {
                printf("Erro no formato do tabuleiro no arquivo.\n");
                fclose(arquivo);
                return;
            }
            tabuleiro[i][j].atual    = c;
            tabuleiro[i][j].original = tolower(c);
        }
    }
    fclose(arquivo);

    printf("Jogo carregado de '%s'.\n", nomeArquivo);
}

void resolver(Celula (*tabuleiro)[MAX_SIZE], int linhas, int colunas) {
    int progress = 1; // Flag to track if progress is being made

    while (progress) {
        progress = 0; // Reset progress flag

        // Iterate through the board to apply rules
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (tabuleiro[i][j].atual == '.') { // Check empty cells
                    int tempViolWhite = 0, tempViolBlocked = 0;

                    // Try painting the cell white
                    tabuleiro[i][j].atual = toupper(tabuleiro[i][j].original);
                    verificarDuplicados(i, j, linhas, colunas, &tempViolWhite);
                    verificarReplicas(i, j, linhas, colunas, &tempViolWhite);
                    verificarVizinhos(i, j, linhas, colunas, &tempViolWhite);
                    verificarConectividade(linhas, colunas, &tempViolWhite);

                    // If painting white causes violations, try blocking the cell
                    if (tempViolWhite > 0) {
                        tabuleiro[i][j].atual = '#'; // Mark as blocked
                        verificarDuplicados(i, j, linhas, colunas, &tempViolBlocked);
                        verificarReplicas(i, j, linhas, colunas, &tempViolBlocked);
                        verificarVizinhos(i, j, linhas, colunas, &tempViolBlocked);
                        verificarConectividade(linhas, colunas, &tempViolBlocked);

                        // If blocking also causes violations, revert to empty
                        if (tempViolBlocked > 0) {
                            tabuleiro[i][j].atual = '.';
                        } else {
                            progress = 1; // Progress made by blocking
                        }
                    } else {
                        progress = 1; // Progress made by painting white
                    }
                }
            }
        }

        // Check if the board is fully solved (no empty cells)
        int solved = 1;
        for (int i = 0; i < linhas && solved; i++) {
            for (int j = 0; j < colunas && solved; j++) {
                if (tabuleiro[i][j].atual == '.') {
                    solved = 0; // Found an empty cell, not solved yet
                }
            }
        }

        if (solved) {
            printf("Tabuleiro resolvido com sucesso!\n");
            return;
        }
    }

    // If no progress is made and the board is not solved, print a message
    printf("Não foi possível resolver o tabuleiro completamente.\n");
}