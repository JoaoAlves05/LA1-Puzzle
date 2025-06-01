#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"
#include "verificacoes.h"
#include "historico.h"

// Vetor de direções ortogonais: cima, baixo, esquerda, direita
int D[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

//Verifica se uma casa riscada tem pelo menos um vizinho branco (maiúscula)
int check_vizinhos_validos(int i, int j, int linhas, int colunas) {
    for (int d = 0; d < 4; d++) {
        int ni = i + D[d][0], nj = j + D[d][1];
        if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas) {
            if (isupper(tabuleiro[ni][nj].atual)) return 1;
        }
    }
    return 0;
}

// Verifica se todas as casas brancas estão conectadas
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

    // Cria matriz de casas visitadas para BFS
    int **visit = malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        visit[i] = calloc(colunas, sizeof(int));
    }

    int queue[linhas*colunas][2];
    int head = 0, tail = 0;
    int contador = 1; // já visitámos a célula inicial

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
                contador++;
            }
        }
    }

    for (int i = 0; i < linhas; i++) free(visit[i]);
    free(visit);

    // Se todas as casas brancas foram visitadas, não há "ilhas"
    return (contador == total_brancas) ? 0 : 1;
}

// Risca casas duplicadas na mesma linha ou coluna
int riscar_duplicados(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (isupper(tabuleiro[i][j].atual)) {
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
    }
    return alteracoes;
}

// Pinta de branco casas vizinhas minúsculas de casas riscadas
int pintar_vizinhos_riscados(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual == '#') {
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
    }
    return alteracoes;
}

// Pinta de branco casas que são a única possibilidade para uma letra numa linha ou coluna
int pintar_replicas_unicas(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual != '#' && !isupper(tabuleiro[i][j].atual)) {
                char letra = tabuleiro[i][j].original;
                int count_row = 0, count_col = 0;
                for (int x = 0; x < colunas; x++) {
                    if (x != j && tolower(tabuleiro[i][x].original) == letra &&
                        tabuleiro[i][x].atual != '#') {
                        count_row++;
                    }
                }
                for (int y = 0; y < linhas; y++) {
                    if (y != i && tolower(tabuleiro[y][j].original) == letra &&
                        tabuleiro[y][j].atual != '#') {
                        count_col++;
                    }
                }
                if (count_row == 0 || count_col == 0) {
                    char ant = tabuleiro[i][j].atual;
                    char novo = toupper(letra);
                    tabuleiro[i][j].atual = novo;
                    if (hist) empilhar(hist, i, j, ant, novo);
                    alteracoes++;
                }
            }
        }
    }
    return alteracoes;
}

// Verifica se pintar uma casa de branco isola casas brancas
int esta_isolar(int i, int j, int linhas, int colunas) {
    char original = tabuleiro[i][j].atual;
    tabuleiro[i][j].atual = toupper(tabuleiro[i][j].original);

    int isolada = contarConectividade(linhas, colunas);

    tabuleiro[i][j].atual = original;

    return isolada != 0;
}

// Risca casas que, se fossem pintadas de branco, isolariam casas brancas
int riscar_casas_isoladas(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual != '#' && !isupper(tabuleiro[i][j].atual)) {
                if (esta_isolar(i, j, linhas, colunas)) {
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



// Tenta uma jogada (pintar ou riscar) numa célula e faz backtracking
int tentar_jogada(int i, int j, int linhas, int colunas, PilhaAlteracoes *hist, char novo_valor) {
    char valor_ant = tabuleiro[i][j].atual;
    tabuleiro[i][j].atual = novo_valor;
    if (hist) empilhar(hist, i, j, valor_ant, novo_valor);
    int resultado = 0;
    if (contarConectividade(linhas, colunas) == 0 && resolver_jogo_backtrack(linhas, colunas, hist)) {
        resultado = 1;
    }
    if (!resultado && hist) desfazer(hist);
    tabuleiro[i][j].atual = valor_ant;
    return resultado;
}

// Verifica se o tabuleiro está completamente resolvido
int tabuleiro_resolvido(int linhas, int colunas) {
    if (contarTodasAsViolacoes(linhas, colunas) != 0) return 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual != '#' && !isupper(tabuleiro[i][j].atual)) {
                tabuleiro[i][j].atual = toupper(tabuleiro[i][j].original);
            }
        }
    }
    return 1;
}

int resolver_jogo_backtrack(int linhas, int colunas, PilhaAlteracoes *hist) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual != '#' && !isupper(tabuleiro[i][j].atual)) {
                // Tenta pintar de branco
                if (tentar_jogada(i, j, linhas, colunas, hist, toupper(tabuleiro[i][j].original))) return 1;
                // Tenta riscar
                if (tentar_jogada(i, j, linhas, colunas, hist, '#')) return 1;
                return 0;
            }
        }
    }
    return tabuleiro_resolvido(linhas, colunas);
}

// Conta duplicados numa linha para cada letra do alfabeto
int contarDuplicadosLinha(int linha, int colunas) {
    int count[26] = {0};
    int violacoes = 0;
    for (int j = 0; j < colunas; j++) {
        if (isupper(tabuleiro[linha][j].atual)) {
            int idx = tolower(tabuleiro[linha][j].atual) - 'a';
            if (idx >= 0 && idx < 26) {
                count[idx]++;
            }
        }
    }
    for (int i = 0; i < 26; i++) {
        if (count[i] > 1) violacoes++;
    }
    return violacoes;
}

// Conta duplicados numa coluna para cada letra do alfabeto
int contarDuplicadosColuna(int linhas, int coluna) {
    int count[26] = {0};
    int violacoes = 0;
    for (int i = 0; i < linhas; i++) {
        if (isupper(tabuleiro[i][coluna].atual)) {
            int idx = tolower(tabuleiro[i][coluna].atual) - 'a';
            if (idx >= 0 && idx < 26) {
                count[idx]++;
            }
        }
    }
    for (int i = 0; i < 26; i++) {
        if (count[i] > 1) violacoes++;
    }
    return violacoes;
}

// função principal para contar o número de duplicados em linhas e colunas
int contarDuplicados(int linhas, int colunas) {
    int violacoes = 0;
    for (int i = 0; i < linhas; i++) {
        violacoes += contarDuplicadosLinha(i, colunas);
    }
    for (int j = 0; j < colunas; j++) {
        violacoes += contarDuplicadosColuna(linhas, j);
    }
    return violacoes;
}

// Função para aplicar todas as regras de ajuda
int aplicar_todas_regras(int linhas, int colunas, PilhaAlteracoes *hist) {
    int alteracoes = 0;
    alteracoes += pintar_replicas_unicas(linhas, colunas, hist);
    alteracoes += riscar_duplicados(linhas, colunas, hist);
    alteracoes += pintar_vizinhos_riscados(linhas, colunas, hist);
    alteracoes += riscar_casas_isoladas(linhas, colunas, hist);

    return alteracoes;
}

// Aplica todas as regras repetidamente até não haver mais alterações
int ajuda_repetida(int linhas, int colunas, PilhaAlteracoes *hist) {
    int total = 0;
    int alteracoes = aplicar_todas_regras(linhas, colunas, hist);
    while (alteracoes > 0) {
        total += alteracoes;
        alteracoes = aplicar_todas_regras(linhas, colunas, hist);
    }
    return total;
}

// Conta o número de casas riscadas sem vizinhos brancos
int contarVizinhos(int linhas, int colunas) {
    int count = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].atual == '#') {
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
    }
    return count;
}

// Conta o número total de violações das regras do puzzle
int contarTodasAsViolacoes(int linhas, int colunas) {
    return contarDuplicados(linhas, colunas)
         + contarVizinhos(linhas, colunas)
         + contarConectividade(linhas, colunas);         
         + contarConectividade(linhas, colunas);
}

