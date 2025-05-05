#include <stdio.h>
#include <ctype.h>
#include "tabuleiro.h"
#include "verificacoes.h"

// Direções ortogonais: cima, baixo, esquerda, direita
int D[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

// Verifica duplicados na linha e coluna de uma casa branca
void verificarDuplicados(Celula (*tabuleiro)[MAX_SIZE], int linha, int coluna, int linhas, int colunas, int *viol) {
    char letra = tabuleiro[linha][coluna].atual;
    // Logic here...
}

// Verifica réplicas não riscadas após uma casa branca
void verificarReplicas(Celula (*tabuleiro)[MAX_SIZE], int i, int j, int linhas, int colunas, int *viol) {
    char alvo = tolower(tabuleiro[i][j].atual);
    for (int x = 0; x < colunas; x++) {
        if (x == j) continue;
        if (tabuleiro[i][x].original == alvo && tabuleiro[i][x].atual != '#')
            (*viol)++;
    }
    for (int y = 0; y < linhas; y++) {
        if (y == i) continue;
        if (tabuleiro[y][j].original == alvo && tabuleiro[y][j].atual != '#')
            (*viol)++;
    }
}

// Verifica se casas riscadas têm vizinhos brancos
void verificarVizinhos(Celula (*tabuleiro)[MAX_SIZE], int i, int j, int linhas, int colunas, int *viol) {
    for (int d = 0; d < 4; d++) {
        int ni = i + D[d][0], nj = j + D[d][1];
        if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas) {
            if (!isupper(tabuleiro[ni][nj].atual))
                (*viol)++;
        }
    }
}

// Verifica conectividade ortogonal entre todas as casas brancas
void verificarConectividade(Celula (*tabuleiro)[MAX_SIZE], int linhas, int colunas, int *viol) {
    int total = 0, si = -1, sj = -1;
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            if (isupper(tabuleiro[i][j].atual)) {
                total++;
                if (si < 0) { si = i; sj = j; }
            }
    if (total <= 1) return;

    int visit[MAX_SIZE][MAX_SIZE] = {{0}};
    int queue[MAX_SIZE*MAX_SIZE][2], head = 0, tail = 0, count = 1;
    visit[si][sj] = 1;
    queue[tail][0] = si; queue[tail][1] = sj; tail++;

    while (head < tail) {
        int i = queue[head][0], j = queue[head][1]; head++;
        for (int d = 0; d < 4; d++) {
            int ni = i + D[d][0], nj = j + D[d][1];
            if (ni>=0 && ni<linhas && nj>=0 && nj<colunas &&
                isupper(tabuleiro[ni][nj].atual) && !visit[ni][nj]) {
                visit[ni][nj] = 1;
                queue[tail][0] = ni; queue[tail][1] = nj; tail++;
                count++;
            }
        }
    }
    if (count != total) (*viol)++;
}

int contarViolacoes(Celula (*tabuleiro)[MAX_SIZE], int linhas, int colunas) {
    int viol = 0;
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++) {
            if (isupper(tabuleiro[i][j].atual)) {
                verificarDuplicados(tabuleiro, i, j, linhas, colunas, &viol);
                verificarReplicas(tabuleiro, i, j, linhas, colunas, &viol);
            } else if (tabuleiro[i][j].atual == '#') {
                verificarVizinhos(tabuleiro, i, j, linhas, colunas, &viol);
            }
        }
    verificarConectividade(tabuleiro, linhas, colunas, &viol);
    return viol;
}

void verificarEstado(Celula (*tabuleiro)[MAX_SIZE], int linhas, int colunas) {
    int viol = contarViolacoes(tabuleiro, linhas, colunas);
    if (viol == 0)
        printf("\nTabuleiro válido! Todas as regras foram cumpridas.\n");
    else
        printf("\nTotal de problemas encontrados: %d\n", viol);
}
