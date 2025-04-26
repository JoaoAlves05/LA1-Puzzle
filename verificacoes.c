#include <stdio.h>
#include <ctype.h>
#include "verificacoes.h"
#include "tabuleiro.h"

// Direções para linhas/colunas: [0] = linha, [1] = coluna
int dir_linha_coluna[2][2] = {{0, 1}, {1, 0}};

// Direções para vizinhos: cima, baixo, esquerda, direita
int dir_vizinhos[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

void verificarDuplicatas(int i, int j, int linhas, int colunas, int *violacoes) {
    char letra = tabuleiro[i][j].atual;
    
    for(int direcao = 0; direcao < 2; direcao++) {
        int count = 0;
        int limite;
        int verificando_linha = (direcao == 0); // 0=linha, 1=coluna

        if(verificando_linha) {
            limite = colunas;
            for(int k = 0; k < limite; k++) {
                if(tabuleiro[i][k].atual == letra) count++;
            }
        } else {
            limite = linhas;
            for(int k = 0; k < limite; k++) {
                if(tabuleiro[k][j].atual == letra) count++;
            }
        }

        if(count > 1) {
            if(verificando_linha) {
                printf("Erro: Letra '%c' repetida %d vezes na linha %d\n", letra, count, i+1);
            } else {
                printf("Erro: Letra '%c' repetida %d vezes na coluna %d\n", letra, count, j+1);
            }
            (*violacoes)++;
        }
    }
}

void verificarReplicas(int i, int j, int linhas, int colunas, int *violacoes) {
    char letra_alvo = tolower(tabuleiro[i][j].atual);
    
    for(int direcao = 0; direcao < 2; direcao++) {
        int verificando_linha = (direcao == 0); // 0=linha, 1=coluna
        
        if(verificando_linha) {
            for(int k = 0; k < colunas; k++) {
                if(k == j) continue;
                if(tabuleiro[i][k].original == letra_alvo && tabuleiro[i][k].atual != '#') {
                    printf("Erro: Réplica '%c' não riscada na linha %d, coluna %d\n", letra_alvo, i+1, k+1);
                    (*violacoes)++;
                }
            }
        } else {
            for(int k = 0; k < linhas; k++) {
                if(k == i) continue;
                if(tabuleiro[k][j].original == letra_alvo && tabuleiro[k][j].atual != '#') {
                    printf("Erro: Réplica '%c' não riscada na coluna %d, linha %d\n", letra_alvo, j+1, k+1);
                    (*violacoes)++;
                }
            }
        }
    }
}

void verificarVizinhos(int i, int j, int linhas, int colunas, int *violacoes) {
    char* lados[] = {"cima", "baixo", "esquerda", "direita"};
    
    for(int d = 0; d < 4; d++) {
        int ni = i + dir_vizinhos[d][0];
        int nj = j + dir_vizinhos[d][1];
        
        if(ni >= 0 && ni < linhas && nj >= 0 && nj < colunas) {
            if(!isupper(tabuleiro[ni][nj].atual)) {
                printf("Erro: Casa (%d,%d) tem vizinho inválido à %s\n", i+1, j+1, lados[d]);
                (*violacoes)++;
            }
        }
    }
}

void verificarEstado(int linhas, int colunas) {
    int violacoes = 0;
    
    for(int i = 0; i < linhas; i++) {
        for(int j = 0; j < colunas; j++) {
            if(isupper(tabuleiro[i][j].atual)) {
                verificarDuplicatas(i, j, linhas, colunas, &violacoes);
                verificarReplicas(i, j, linhas, colunas, &violacoes);
            }
            if(tabuleiro[i][j].atual == '#') {
                verificarVizinhos(i, j, linhas, colunas, &violacoes);
            }
        }
    }
    
    if(violacoes == 0) {
        printf("\nTabuleiro válido! Todas as regras foram cumpridas.\n");
    } else {
        printf("\nTotal de problemas encontrados: %d\n", violacoes);
    }
}