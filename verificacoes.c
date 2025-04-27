#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "tabuleiro.h"
#include "verificacoes.h"

static int D[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

static void verificarDuplicados(int i, int j, int linhas,
                                int colunas, int *viol)
{
    char letra = tabuleiro[i][j].atual;
    int cnt = 0;
    // linha
    for (int x = 0; x < colunas; x++)
        if (tabuleiro[i][x].atual == letra)
            cnt++;
    if (cnt > 1) {
        printf("Erro: Letra '%c' repetida %d vezes na linha %d\n",
               letra, cnt, i+1);
        (*viol)++;
    }
    // coluna
    cnt = 0;
    for (int y = 0; y < linhas; y++)
        if (tabuleiro[y][j].atual == letra)
            cnt++;
    if (cnt > 1) {
        printf("Erro: Letra '%c' repetida %d vezes na coluna %d\n",
               letra, cnt, j+1);
        (*viol)++;
    }
}

static void verificarReplicas(int i, int j, int linhas,
                              int colunas, int *viol)
{
    char alvo = tolower(tabuleiro[i][j].atual);
    for (int x = 0; x < colunas; x++) {
        if (x == j) continue;
        if (tabuleiro[i][x].original == alvo &&
            tabuleiro[i][x].atual != '#') {
            printf("Erro: Réplica '%c' não riscada na linha %d, coluna %d\n",
                   alvo, i+1, x+1);
            (*viol)++;
        }
    }
    for (int y = 0; y < linhas; y++) {
        if (y == i) continue;
        if (tabuleiro[y][j].original == alvo &&
            tabuleiro[y][j].atual != '#') {
            printf("Erro: Réplica '%c' não riscada na coluna %d, linha %d\n",
                   alvo, j+1, y+1);
            (*viol)++;
        }
    }
}

static void verificarVizinhos(int i, int j, int linhas,
                              int colunas, int *viol)
{
    for (int d = 0; d < 4; d++) {
        int ni = i + D[d][0], nj = j + D[d][1];
        if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas) {
            if (!isupper(tabuleiro[ni][nj].atual)) {
                const char *dirName = (d==0?"acima":d==1?"abaixo":d==2?"à esquerda":"à direita");
                printf("Erro: Casa (%d,%d) tem vizinho inválido %s\n",
                       i+1, j+1, dirName);
                (*viol)++;
            }
        }
    }
}

static void verificarConectividade(int linhas, int colunas, int *viol)
{
    // conta brancas e encontra ponto inicial
    int total = 0;
    int si = -1, sj = -1;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (isupper(tabuleiro[i][j].atual)) {
                total++;
                if (si < 0) { si = i; sj = j; }
            }
        }
    }
    if (total < 2) return;  // nada a conectar ou só uma

    // BFS
    int visit[MAX_SIZE][MAX_SIZE] = {{0}};
    int queue[MAX_SIZE*MAX_SIZE][2];
    int head = 0, tail = 0;

    queue[tail][0] = si;
    queue[tail][1] = sj;
    tail++;
    visit[si][sj] = 1;
    int count = 1;

    while (head < tail) {
        int i = queue[head][0];
        int j = queue[head][1];
        head++;
        for (int d = 0; d < 4; d++) {
            int ni = i + D[d][0];
            int nj = j + D[d][1];
            if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas) {
                if (isupper(tabuleiro[ni][nj].atual) && !visit[ni][nj]) {
                    visit[ni][nj] = 1;
                    queue[tail][0] = ni;
                    queue[tail][1] = nj;
                    tail++;
                    count++;
                }
            }
        }
    }
    if (count != total) {
        printf("Erro: Casas brancas não estão conectadas ortogonalmente\n");
        (*viol)++;
    }
}

void verificarEstado(int linhas, int colunas)
{
    int viol = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (isupper(tabuleiro[i][j].atual)) {
                verificarDuplicados(i, j, linhas, colunas, &viol);
                verificarReplicas(i, j, linhas, colunas, &viol);
            } else if (tabuleiro[i][j].atual == '#') {
                verificarVizinhos(i, j, linhas, colunas, &viol);
            }
        }
    }
    verificarConectividade(linhas, colunas, &viol);

    if (viol == 0)
        printf("\nTabuleiro válido! Todas as regras foram cumpridas.\n");
    else
        printf("\nTotal de problemas encontrados: %d\n", viol);
}