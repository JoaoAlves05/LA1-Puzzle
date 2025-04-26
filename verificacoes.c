#include <stdio.h>
#include <ctype.h>
#include "verificacoes.h"
#include "tabuleiro.h"

// Direções para linha/coluna (horizontal e vertical)
int dir_linha_coluna[2][2] = {{0, 1}, {1, 0}};

// Direções para os quatro vizinhos: cima, baixo, esquerda, direita
int dir_vizinhos[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Verifica duplicados na linha ou coluna
void verificarDuplicados(int i, int j, int linhas, int colunas, int *violacoes) {
    char letra = tabuleiro[i][j].atual;

    for (int direcao = 0; direcao < 2; direcao++) {
        int count = 0;
        int limite;
        if (direcao == 0){
            limite = colunas;
        }else
            limite = linhas;

        for (int k = 0; k < limite; k++) {
            char atual;
            if (direcao == 0){
                atual = tabuleiro[i][k].atual;
            }else{
                atual = tabuleiro[k][j].atual;
    }
            if (atual == letra) count++;
        }

        if (count > 1) {
            if (direcao == 0)
                printf("Erro: Letra '%c' repetida %d vezes na linha %d\n", letra, count, i + 1);
            else
                printf("Erro: Letra '%c' repetida %d vezes na coluna %d\n", letra, count, j + 1);

            (*violacoes)++;
        }
    }
}

// Verifica se há réplicas (cópias da letra original) não riscadas
void verificarReplicas(int i, int j, int linhas, int colunas, int *violacoes) {
    char letra_alvo = tolower(tabuleiro[i][j].atual);

    for (int direcao = 0; direcao < 2; direcao++) {
        int limite;
        if (direcao == 0)
            limite = colunas;
        else
            limite = linhas;

        for (int k = 0; k < limite; k++) {
            if ((direcao == 0 && k == j) || (direcao == 1 && k == i)) continue;

            char original, atual;
            if (direcao == 0) {
                original = tabuleiro[i][k].original;
                atual = tabuleiro[i][k].atual;
            } else {
                original = tabuleiro[k][j].original;
                atual = tabuleiro[k][j].atual;
            }

            if (original == letra_alvo && atual != '#') {
                if (direcao == 0)
                    printf("Erro: Réplica '%c' não riscada na linha %d, coluna %d\n", letra_alvo, i + 1, k + 1);
                else
                    printf("Erro: Réplica '%c' não riscada na coluna %d, linha %d\n", letra_alvo, j + 1, k + 1);

                (*violacoes)++;
            }
        }
    }
}

// Verifica se as casas riscadas têm vizinhos válidos (maiúsculos)
void verificarVizinhos(int i, int j, int linhas, int colunas, int *violacoes) {
    for (int d = 0; d < 4; d++) {
        int ni = i + dir_vizinhos[d][0];
        int nj = j + dir_vizinhos[d][1];

        if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas) {
            if (!isupper(tabuleiro[ni][nj].atual)) {
                if (d == 0)
                    printf("Erro: Casa (%d,%d) tem vizinho inválido acima\n", i + 1, j + 1);
                else if (d == 1)
                    printf("Erro: Casa (%d,%d) tem vizinho inválido abaixo\n", i + 1, j + 1);
                else if (d == 2)
                    printf("Erro: Casa (%d,%d) tem vizinho inválido à esquerda\n", i + 1, j + 1);
                else if (d == 3)
                    printf("Erro: Casa (%d,%d) tem vizinho inválido à direita\n", i + 1, j + 1);

                (*violacoes)++;
            }
        }
    }
}

// Verifica o estado completo do tabuleiro
void verificarEstado(int linhas, int colunas) {
    int violacoes = 0;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (isupper(tabuleiro[i][j].atual)) {
                verificarDuplicados(i, j, linhas, colunas, &violacoes);
                verificarReplicas(i, j, linhas, colunas, &violacoes);
            }
            if (tabuleiro[i][j].atual == '#') {
                verificarVizinhos(i, j, linhas, colunas, &violacoes);
            }
        }
    }

    if (violacoes == 0)
        printf("\nTabuleiro válido! Todas as regras foram cumpridas.\n");
    else
        printf("\nTotal de problemas encontrados: %d\n", violacoes);
}
