#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"
#include "verificacoes.h"
#include "historico.h"

// Orthogonal directions (up, down, left, right)
const int D[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

// Helper function to check if a crossed cell has white neighbors
int check_vizinhos_validos(int i, int j, int linhas, int colunas) {
    for (int d = 0; d < 4; d++) {
        int ni = i + D[d][0], nj = j + D[d][1];
        if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas) {
            if (isupper(tabuleiro[ni][nj].atual)) {
                return 1;
            }
        }
    }
    return 0;
}

int contarConectividade(int linhas, int colunas) {
    int total_brancas = 0;
    int start_i = -1, start_j = -1;

    // Só células isupper() contam como brancas
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (isupper(tabuleiro[i][j].atual)) {
                total_brancas++;
                if (start_i < 0) {
                    start_i = i;
                    start_j = j;
                }
            }
        }
    }

    if (total_brancas <= 1) return 0;

    int **visit = malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        visit[i] = calloc(colunas, sizeof(int));
    }

    int queue[linhas*colunas][2];
    int head = 0, tail = 0;
    int count = 1; // já visitamos a célula inicial

    visit[start_i][start_j] = 1;
    queue[tail][0] = start_i;
    queue[tail][1] = start_j;
    tail++;

    while (head < tail) {
        int i = queue[head][0];
        int j = queue[head][1];
        head++;

        for (int d = 0; d < 4; d++) {
            int ni = i + D[d][0];
            int nj = j + D[d][1];

            if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas &&
                isupper(tabuleiro[ni][nj].atual) && !visit[ni][nj]) {
                visit[ni][nj] = 1;
                queue[tail][0] = ni;
                queue[tail][1] = nj;
                tail++;
                count++;
            }
        }
    }

    for (int i = 0; i < linhas; i++) free(visit[i]);
    free(visit);

    return (count == total_brancas) ? 0 : 1;
}

int riscar_duplicados(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (!isupper(tabuleiro[i][j].atual)) continue;
            
            char letra = tolower(tabuleiro[i][j].atual);
            
            // Check row
            for (int x = 0; x < colunas; x++) {
                if (x != j && tolower(tabuleiro[i][x].atual) == letra) {
                    if (tabuleiro[i][x].atual != '#') {
                        char ant = tabuleiro[i][x].atual;
                        tabuleiro[i][x].atual = '#';
                        if (hist) empilhar(hist, i, x, ant, '#');
                        alteracoes++;
                    }
                }
            }
            
            // Check column
            for (int y = 0; y < linhas; y++) {
                if (y != i && tolower(tabuleiro[y][j].atual) == letra) {
                    if (tabuleiro[y][j].atual != '#') {
                        char ant = tabuleiro[y][j].atual;
                        tabuleiro[y][j].atual = '#';
                        if (hist) empilhar(hist, y, j, ant, '#');
                        alteracoes++;
                    }
                }
            }
        }
    }
    return alteracoes;
}

int pintar_vizinhos_riscados(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual != '#') continue;
            
            for (int d = 0; d < 4; d++) {
                int ni = i + D[d][0];
                int nj = j + D[d][1];
                
                if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas &&
                    islower(tabuleiro[ni][nj].atual)) {
                    char ant = tabuleiro[ni][nj].atual;
                    char novo = toupper(tabuleiro[ni][nj].original);
                    tabuleiro[ni][nj].atual = novo;
                    if (hist) empilhar(hist, ni, nj, ant, novo);
                    alteracoes++;
                }
            }
        }
    }
    return alteracoes;
}

int pintar_replicas_unicas(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual == '#' || isupper(tabuleiro[i][j].atual)) continue;
            
            char letra = tabuleiro[i][j].original;
            int count_row = 0, count_col = 0;
            
            // Count possible positions in row
            for (int x = 0; x < colunas; x++) {
                if (x != j && tolower(tabuleiro[i][x].original) == letra &&
                    tabuleiro[i][x].atual != '#') {
                    count_row++;
                }
            }
            
            // Count possible positions in column
            for (int y = 0; y < linhas; y++) {
                if (y != i && tolower(tabuleiro[y][j].original) == letra &&
                    tabuleiro[y][j].atual != '#') {
                    count_col++;
                }
            }
            
            // If unique possible position
            if (count_row == 0 || count_col == 0) {
                char ant = tabuleiro[i][j].atual;
                char novo = toupper(letra);
                tabuleiro[i][j].atual = novo;
                if (hist) empilhar(hist, i, j, ant, novo);
                alteracoes++;
            }
        }
    }
    return alteracoes;
}

// Aplica as regras de ajuda a todas as casas possíveis, uma única passagem
int ajuda_automatica(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    // Percorre o tabuleiro inteiro e aplica as regras a todas as casas possíveis
    alteracoes += pintar_replicas_unicas(linhas, colunas, hist);
    alteracoes += riscar_duplicados(linhas, colunas, hist);
    alteracoes += pintar_vizinhos_riscados(linhas, colunas, hist);
    return alteracoes;
}

// Aplica ajuda_automatica repetidamente até não haver mais alterações
int ajuda_repetida(int linhas, int colunas, PilhaAlteracoes *hist) {
    int total = 0;
    int alteracoes;
    do {
        alteracoes = ajuda_automatica(linhas, colunas, hist);
        total += alteracoes;
    } while (alteracoes > 0);
    return total;
}

// Backtracking exaustivo otimizado: usa matriz de fixas para evitar recomputações
int resolver_jogo_backtrack(int linhas, int colunas, PilhaAlteracoes *hist) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual != '#' && !isupper(tabuleiro[i][j].atual)) {
                char valor_ant = tabuleiro[i][j].atual;
                // 1. Tenta pintar de branco
                tabuleiro[i][j].atual = toupper(tabuleiro[i][j].original);
                if (hist) empilhar(hist, i, j, valor_ant, tabuleiro[i][j].atual);
                if (resolver_jogo_backtrack(linhas, colunas, hist)) return 1;
                if (hist) desfazer(hist);
                tabuleiro[i][j].atual = valor_ant;
                // 2. Tenta riscar
                tabuleiro[i][j].atual = '#';
                if (hist) empilhar(hist, i, j, valor_ant, '#');
                if (resolver_jogo_backtrack(linhas, colunas, hist)) return 1;
                if (hist) desfazer(hist);
                tabuleiro[i][j].atual = valor_ant;
                return 0;
            }
        }
    }
    // Só aqui, quando não há mais casas livres, verifica se está resolvido
    if (contarTodasAsViolacoes(linhas, colunas) == 0) {
        // Pinta todas as casas não riscadas de branco (maiúsculas)
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (tabuleiro[i][j].atual != '#' && !isupper(tabuleiro[i][j].atual)) {
                    tabuleiro[i][j].atual = toupper(tabuleiro[i][j].original);
                }
            }
        }
        return 1;
    }
    return 0;
}

int resolver_jogo(int linhas, int colunas, PilhaAlteracoes *hist) {
    return resolver_jogo_backtrack(linhas, colunas, hist);
}




int contarVizinhos(int linhas, int colunas) {
    int count = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual != '#') continue;
            
            int valid = 0;
            for (int d = 0; d < 4; d++) {
                int ni = i + D[d][0];
                int nj = j + D[d][1];
                if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas &&
                    isupper(tabuleiro[ni][nj].atual)) {
                    valid = 1;
                    break;
                }
            }
            if (!valid) count++;
        }
    }
    return count;
}

int contarDuplicados(int linhas, int colunas) {
    int violacoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (!isupper(tabuleiro[i][j].atual)) continue;
            char letra = tolower(tabuleiro[i][j].atual);
            // Check row duplicates
            for (int x = 0; x < colunas; x++) {
                if (x != j && tolower(tabuleiro[i][x].atual) == letra && isupper(tabuleiro[i][x].atual)) {
                    violacoes++;
                }
            }
            // Check column duplicates
            for (int y = 0; y < linhas; y++) {
                if (y != i && tolower(tabuleiro[y][j].atual) == letra && isupper(tabuleiro[y][j].atual)) {
                    violacoes++;
                }
            }
        }
    }
    return violacoes;
}

int contarTodasAsViolacoes(int linhas, int colunas) {
    return contarDuplicados(linhas, colunas)
         + contarVizinhos(linhas, colunas)
         + contarConectividade(linhas, colunas);
}

