#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"
#include "verificacoes.h"
#include "historico.h"

// Vetor de direções ortogonais: cima, baixo, esquerda, direita
const int D[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

// Função auxiliar: verifica se uma casa riscada tem pelo menos um vizinho branco (maiúscula)
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

// Verifica se todas as casas brancas estão conectadas (não existem ilhas)
int contarConectividade(int linhas, int colunas) {
    int total_brancas = 0;
    int start_i = -1, start_j = -1;

    // Conta o número de casas brancas (maiúsculas) e encontra a primeira
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

    // Cria matriz de visitados para BFS
    int **visit = malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        visit[i] = calloc(colunas, sizeof(int));
    }

    int queue[linhas*colunas][2];
    int head = 0, tail = 0;
    int count = 1; // já visitámos a célula inicial

    visit[start_i][start_j] = 1;
    queue[tail][0] = start_i;
    queue[tail][1] = start_j;
    tail++;

    // BFS para visitar todas as casas brancas conectadas
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

    // Se todas as casas brancas foram visitadas, não há ilhas
    return (count == total_brancas) ? 0 : 1;
}

// Risca casas duplicadas na mesma linha ou coluna
int riscar_duplicados(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (!isupper(tabuleiro[i][j].atual)) continue;
            
            char letra = tolower(tabuleiro[i][j].atual);
            
            // Procura duplicados na linha
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
            
            // Procura duplicados na coluna
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

// Pinta de branco casas vizinhas minúsculas de casas riscadas
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

// Pinta de branco casas que são a única possibilidade para uma letra numa linha ou coluna
int pintar_replicas_unicas(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual == '#' || isupper(tabuleiro[i][j].atual)) continue;
            
            char letra = tabuleiro[i][j].original;
            int count_row = 0, count_col = 0;
            
            // Conta possíveis posições na linha
            for (int x = 0; x < colunas; x++) {
                if (x != j && tolower(tabuleiro[i][x].original) == letra &&
                    tabuleiro[i][x].atual != '#') {
                    count_row++;
                }
            }
            
            // Conta possíveis posições na coluna
            for (int y = 0; y < linhas; y++) {
                if (y != i && tolower(tabuleiro[y][j].original) == letra &&
                    tabuleiro[y][j].atual != '#') {
                    count_col++;
                }
            }
            
            // Se for posição única, pinta de branco
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

// Verifica se pintar uma casa de branco isola casas brancas
int is_isolating(int i, int j, int linhas, int colunas) {
    char original = tabuleiro[i][j].atual;
    tabuleiro[i][j].atual = toupper(tabuleiro[i][j].original);

    int isolated = contarConectividade(linhas, colunas);

    tabuleiro[i][j].atual = original;

    return isolated != 0;
}

// Risca casas que, se fossem pintadas de branco, isolariam casas brancas
int riscar_casas_isoladas(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual != '#' && !isupper(tabuleiro[i][j].atual)) {
                if (is_isolating(i, j, linhas, colunas)) {
                    char ant = tabuleiro[i][j].atual;
                    tabuleiro[i][j].atual = '#';
                    if (hist) empilhar(hist, i, j, ant, '#');
                    alteracoes++;
                }
            }
        }
    }
    return alteracoes;
}

// Risca casas forçadas por adjacência de letras iguais em linhas ou colunas
int riscar_forcados_por_adjacencia(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    // Verifica linhas para letras adjacentes iguais
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas - 1; j++) {
            if (tabuleiro[i][j].atual != '#' && tabuleiro[i][j+1].atual != '#') {
                char letra1 = tolower(tabuleiro[i][j].original);
                char letra2 = tolower(tabuleiro[i][j+1].original);
                if (letra1 == letra2) {
                    // Risca uma das casas se a outra já estiver pintada de branco
                    if (!isupper(tabuleiro[i][j].atual) && isupper(tabuleiro[i][j+1].atual)) {
                        char ant = tabuleiro[i][j].atual;
                        tabuleiro[i][j].atual = '#';
                        if (hist) empilhar(hist, i, j, ant, '#');
                        alteracoes++;
                    } else if (isupper(tabuleiro[i][j].atual) && !isupper(tabuleiro[i][j+1].atual)) {
                        char ant = tabuleiro[i][j+1].atual;
                        tabuleiro[i][j+1].atual = '#';
                        if (hist) empilhar(hist, i, j+1, ant, '#');
                        alteracoes++;
                    }
                }
            }
        }
    }
    // Verifica colunas para letras adjacentes iguais
    for (int j = 0; j < colunas; j++) {
        for (int i = 0; i < linhas - 1; i++) {
            if (tabuleiro[i][j].atual != '#' && tabuleiro[i+1][j].atual != '#') {
                char letra1 = tolower(tabuleiro[i][j].original);
                char letra2 = tolower(tabuleiro[i+1][j].original);
                if (letra1 == letra2) {
                    if (!isupper(tabuleiro[i][j].atual) && isupper(tabuleiro[i+1][j].atual)) {
                        char ant = tabuleiro[i][j].atual;
                        tabuleiro[i][j].atual = '#';
                        if (hist) empilhar(hist, i, j, ant, '#');
                        alteracoes++;
                    } else if (isupper(tabuleiro[i][j].atual) && !isupper(tabuleiro[i+1][j].atual)) {
                        char ant = tabuleiro[i+1][j].atual;
                        tabuleiro[i+1][j].atual = '#';
                        if (hist) empilhar(hist, i+1, j, ant, '#');
                        alteracoes++;
                    }
                }
            }
        }
    }
    return alteracoes;
}

// Aplica todas as regras de ajuda ao tabuleiro numa única passagem
int ajuda_automatica(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    alteracoes += pintar_replicas_unicas(linhas, colunas, hist);
    alteracoes += riscar_duplicados(linhas, colunas, hist);
    alteracoes += pintar_vizinhos_riscados(linhas, colunas, hist);
    alteracoes += riscar_casas_isoladas(linhas, colunas, hist);
    alteracoes += riscar_forcados_por_adjacencia(linhas, colunas, hist);
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

// Resolução automática do puzzle por backtracking exaustivo
int resolver_jogo_backtrack(int linhas, int colunas, PilhaAlteracoes *hist) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual != '#' && !isupper(tabuleiro[i][j].atual)) {
                char valor_ant = tabuleiro[i][j].atual;
                // Tenta pintar de branco
                tabuleiro[i][j].atual = toupper(tabuleiro[i][j].original);
                if (hist) empilhar(hist, i, j, valor_ant, tabuleiro[i][j].atual);
                if (contarConectividade(linhas, colunas) == 0 && resolver_jogo_backtrack(linhas, colunas, hist)) return 1;
                if (hist) desfazer(hist);
                tabuleiro[i][j].atual = valor_ant;
                // Tenta riscar
                tabuleiro[i][j].atual = '#';
                if (hist) empilhar(hist, i, j, valor_ant, '#');
                if (contarConectividade(linhas, colunas) == 0 && resolver_jogo_backtrack(linhas, colunas, hist)) return 1;
                if (hist) desfazer(hist);
                tabuleiro[i][j].atual = valor_ant;
                return 0;
            }
        }
    }
    // Quando não há mais casas livres, verifica se está resolvido
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

// Conta o número de casas riscadas sem vizinhos brancos
int contarVizinhos(int linhas, int colunas) {
    int count = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual != '#') continue;
            
            int valido = 0;
            for (int d = 0; d < 4 && !valido; d++) {
                int ni = i + D[d][0];
                int nj = j + D[d][1];
                if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas &&
                    isupper(tabuleiro[ni][nj].atual)) {
                    valido = 1;
                }
            }
            if (!valido) count++;
        }
    }
    return count;
}

// Conta o número de duplicados (letras repetidas) em linhas e colunas
int contarDuplicados(int linhas, int colunas) {
    int violacoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (!isupper(tabuleiro[i][j].atual)) continue;
            char letra = tolower(tabuleiro[i][j].atual);
            // Verifica duplicados na linha
            for (int x = 0; x < colunas; x++) {
                if (x != j && tolower(tabuleiro[i][x].atual) == letra && isupper(tabuleiro[i][x].atual)) {
                    violacoes++;
                }
            }
            // Verifica duplicados na coluna
            for (int y = 0; y < linhas; y++) {
                if (y != i && tolower(tabuleiro[y][j].atual) == letra && isupper(tabuleiro[y][j].atual)) {
                    violacoes++;
                }
            }
        }
    }
    return violacoes;
}

// Conta o número total de violações das regras do puzzle
int contarTodasAsViolacoes(int linhas, int colunas) {
    return contarDuplicados(linhas, colunas)
         + contarVizinhos(linhas, colunas)
         + contarConectividade(linhas, colunas);
}

