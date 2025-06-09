#include "tabuleiro.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Definição do tabuleiro global
Celula **tabuleiro = NULL; // Ponteiro para a matriz do tabuleiro
int tabuleiro_linhas = 0;  // Número de linhas do tabuleiro

// Inicializa o tabuleiro, alocando memória para as linhas e colunas
void inicializarTabuleiro(int linhas, int colunas) {
    tabuleiro = malloc(linhas * sizeof(Celula *));
    if (!tabuleiro) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o tabuleiro.\n");
        exit(1);
    }
    for (int i = 0; i < linhas; i++) {
        tabuleiro[i] = malloc(colunas * sizeof(Celula));
        if (!tabuleiro[i]) {
            fprintf(stderr, "Erro: Falha ao alocar memória para linha do tabuleiro.\n");
            exit(1);
        }
    }
    tabuleiro_linhas = linhas;
}

// Liberta a memória ocupada pelo tabuleiro
void liberarTabuleiro(int linhas) {
    if (tabuleiro == NULL) return;
    for (int i = 0; i < linhas; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
    tabuleiro = NULL;
    tabuleiro_linhas = 0;
}

// Valida e converte uma coordenada do tipo "a1" para índices de linha e coluna
int input_coordenada(char *coordenada, int *linha, int *coluna, int max_linhas, int max_colunas) {
    // Verifica se a coordenada é válida (primeiro caracter letra, segundo número)
    if (!coordenada || !isalpha(coordenada[0]) || !isdigit(coordenada[1]))
        return 0;

    *coluna = tolower(coordenada[0]) - 'a'; // Converte letra para índice de coluna
    *linha  = atoi(coordenada + 1) - 1;     // Converte número para índice de linha

    // Verifica se está dentro dos limites do tabuleiro
    return (*linha >= 0 && *linha < max_linhas && *coluna >= 0 && *coluna < max_colunas);
}

// Mostra o tabuleiro atual no ecrã
void exibirTabuleiro(int linhas, int colunas) {
    printf("\n   %s", ANSI_BOLD);
    for (int j = 0; j < colunas; j++) {
        printf("%c ", 'a' + j);
    }
    printf("%s\n", ANSI_RESET);
    printf("   ");
    for (int j = 0; j < colunas; j++) printf("--");
    printf("-\n");
    for (int i = 0; i < linhas; i++) {
        printf("%2d| ", i + 1);
        for (int j = 0; j < colunas; j++) {
            char c = tabuleiro[i][j].atual;
            if (c == '#') {
                printf("%s#%s ", ANSI_RED, ANSI_RESET);
            } else if (isupper(c)) {
                printf("%s%c%s ", ANSI_GREEN, c, ANSI_RESET);
            } else {
                printf("%c ", c);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Pinta uma casa de branco (maiúscula), se possível
int pintarDeBranco(int linha, int coluna) {
    char atual = tabuleiro[linha][coluna].atual;
    if (atual == '#') {
        return 0;
    }
    if (isupper(atual)) {
        return 0;
    }
    tabuleiro[linha][coluna].atual = toupper(tabuleiro[linha][coluna].original);
    return 1;
}

// Risca uma casa ('#'), se possível
int riscarCasa(int linha, int coluna) {
    char atual = tabuleiro[linha][coluna].atual;
    if (atual == '#') {
        return 0;
    }
    if (isupper(atual)) {
        return 0;
    }
    tabuleiro[linha][coluna].atual = '#';
    return 1;
}

// Escreve o tabuleiro num ficheiro, linha a linha
void escreverTabuleiro(FILE *arquivo, int linhas, int colunas, char tipo) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            char valor = (tipo == 'O') ? tabuleiro[i][j].original : tabuleiro[i][j].atual;
            fprintf(arquivo, "%c ", valor);
        }
        fprintf(arquivo, "\n");
    }
}

// Lê o tabuleiro de um ficheiro, preenchendo a matriz global
int lerTabuleiro(FILE *arquivo, int linhas, int colunas, char tipo) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            char c;
            if (fscanf(arquivo, " %c", &c) != 1) {
                fprintf(stderr, "Erro: Falha ao ler caractere na linha %d, coluna %d (%s).\n", i + 1, j + 1, tipo == 'O' ? "Original" : "Atual");
                return 0;
            }
            if (!(isalpha(c) || c == '#')) {
                fprintf(stderr, "Erro: Caractere inválido '%c' na linha %d, coluna %d (%s).\n", c, i + 1, j + 1, tipo == 'O' ? "Original" : "Atual");
                return 0;
            }
            if (tipo == 'O') {
                tabuleiro[i][j].original = c;
            } else {
                tabuleiro[i][j].atual = c;
            }
        }
    }
    return 1;
}

// Grava o estado do jogo num ficheiro, incluindo original e atual
void gravarJogo(char *nomeArquivo, int linhas, int colunas) {
    // Garante que o diretório 'saved' existe
    struct stat st = {0};
    if (stat("saved", &st) == -1) {
        mkdir("saved", 0700);
    }
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "saved/%s", nomeArquivo);

    FILE *arquivo = fopen(caminho, "w");
    if (!arquivo) {
        fprintf(stderr, "Erro: Não foi possível criar o arquivo '%s'.\n", caminho);
        return;
    }
    fprintf(arquivo, "%d %d\n", linhas, colunas);
    fprintf(arquivo, "ORIGINAL\n");
    escreverTabuleiro(arquivo, linhas, colunas, 'O');
    fprintf(arquivo, "ATUAL\n");
    escreverTabuleiro(arquivo, linhas, colunas, 'A');
    fclose(arquivo);
    printf("Jogo gravado com sucesso em '%s'.\n", caminho);
}

// Procura uma linha separadora no ficheiro (ex: "ORIGINAL" ou "ATUAL")
int procurar_separador(FILE *arquivo, char *separador) {
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), arquivo)) {
        if (strstr(buffer, separador) != NULL) return 1;
    }
    return 0;
}

// Lê as dimensões do tabuleiro do ficheiro
int ler_dimensoes(FILE *arquivo, int *linhas, int *colunas) {
    return fscanf(arquivo, "%d %d", linhas, colunas) == 2 && *linhas > 0 && *colunas > 0;
}

// Carrega o estado do jogo de um ficheiro, preenchendo o tabuleiro
void carregarJogo(char *nomeArquivo, int *linhas, int *colunas) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "saved/%s", nomeArquivo);

    FILE *arquivo = fopen(caminho, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro: Arquivo '%s' não encontrado.\n", caminho);
        return;
    }
    int novas_linhas, novas_colunas;
    if (!ler_dimensoes(arquivo, &novas_linhas, &novas_colunas)) {
        fprintf(stderr, "Erro: Formato inválido no cabeçalho do arquivo.\n");
        fclose(arquivo);
        return;
    }
    // Liberta o tabuleiro antigo usando o número de linhas antigo
    if (tabuleiro != NULL) {
        liberarTabuleiro(tabuleiro_linhas);
    }
    // Atualiza as variáveis de linhas e colunas para os novos valores
    *linhas = novas_linhas;
    *colunas = novas_colunas;
    inicializarTabuleiro(*linhas, *colunas);
    if (!procurar_separador(arquivo, "ORIGINAL")) {
        fprintf(stderr, "Erro: Separador 'ORIGINAL' não encontrado.\n");
        fclose(arquivo);
        return;
    }
    if (!lerTabuleiro(arquivo, *linhas, *colunas, 'O')) {
        fclose(arquivo);
        return;
    }
    if (!procurar_separador(arquivo, "ATUAL")) {
        fprintf(stderr, "Erro: Separador 'ATUAL' não encontrado.\n");
        fclose(arquivo);
        return;
    }
    if (!lerTabuleiro(arquivo, *linhas, *colunas, 'A')) {
        fclose(arquivo);
        return;
    }
    fclose(arquivo);
    printf("Jogo carregado com sucesso de '%s'.\n", nomeArquivo);
}