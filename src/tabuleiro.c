#include "tabuleiro.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Definição do tabuleiro global
Celula **tabuleiro = NULL; // Inicializado como NULL
int tabuleiro_linhas = 0;

void inicializarTabuleiro(int linhas, int colunas) {
    tabuleiro = malloc(linhas * sizeof(Celula *)); // Aloca memória para as linhas
    for (int i = 0; i < linhas; i++) {
        tabuleiro[i] = malloc(colunas * sizeof(Celula)); // Aloca memória para as colunas
    }
    tabuleiro_linhas = linhas; // Armazena o número de linhas
}

void liberarTabuleiro(int linhas) {
    if (tabuleiro == NULL) return;
    for (int i = 0; i < linhas; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
    tabuleiro = NULL;
    tabuleiro_linhas = 0;
}

int input_coordenada(char *coordenada, int *linha, int *coluna, int max_linhas, int max_colunas) // Função para validar a coordenada dada no input
{
    //se a primeira letra não for letra ou o segundo caracter não for número, é coordenada inválida
    if (!coordenada || !isalpha(coordenada[0]) || !isdigit(coordenada[1]))
        return 0;

    *coluna = tolower(coordenada[0]) - 'a'; //converte a letra(já em minusculo) para número
    *linha  = atoi(coordenada + 1) - 1; //converte para inteiro e subtrai 1 para ficar no range 0 a n-1

    //verifica se a coordenada está dentro dos limites do tabuleiro
    return (*linha >= 0 && *linha < max_linhas && *coluna >= 0 && *coluna < max_colunas);
}

// Função para exibir o tabuleiro(iterando a matriz)
void exibirTabuleiro(int linhas, int colunas)
{
    printf("Tabuleiro atual:\n");
    putchar('\n');
    //Itera o tabuleiro
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j].atual);//Imprime o valor atual de cada célula
        }
        printf("\n");
    }
}

int pintarDeBranco(int linha, int coluna){
    char atual = tabuleiro[linha][coluna].atual;
    if (atual == '#') {
        // printf("Erro: Essa casa já está riscada.\n");
        return 0;
    }
    if (isupper(atual)) {
        // printf("Erro: Essa casa já está pintada de branco.\n");
        return 0;
    }
    tabuleiro[linha][coluna].atual = toupper(tabuleiro[linha][coluna].original); //converte o valor original para maiúsculo
    return 1;
}

int riscarCasa(int linha, int coluna){
    char atual = tabuleiro[linha][coluna].atual; //pega o valor atual da célula
    if (atual == '#') {
        // printf("Erro: Essa casa já está riscada.\n");
        return 0;
    }
    if (isupper(atual)) {
        // printf("Erro: Essa casa já está pintada de branco.\n");
        return 0;
    }
    tabuleiro[linha][coluna].atual = '#';
    return 1;
}

// Função auxiliar para escrever o tabuleiro em um arquivo
void escreverTabuleiro(FILE *arquivo, int linhas, int colunas, char tipo) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            char valor = (tipo == 'O') ? tabuleiro[i][j].original : tabuleiro[i][j].atual;
            fprintf(arquivo, "%c ", valor);
        }
        fprintf(arquivo, "\n");
    }
}

// Função auxiliar para ler o tabuleiro de um arquivo
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

void gravarJogo(char *nomeArquivo, int linhas, int colunas) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        fprintf(stderr, "Erro: Não foi possível criar o arquivo '%s'.\n", nomeArquivo);
        return;
    }
    fprintf(arquivo, "%d %d\n", linhas, colunas); // Escreve dimensões
    fprintf(arquivo, "ORIGINAL\n"); // Escreve cabeçalho sem hífens
    escreverTabuleiro(arquivo, linhas, colunas, 'O'); // Escreve tabuleiro original
    fprintf(arquivo, "ATUAL\n"); // Escreve cabeçalho sem hífens
    escreverTabuleiro(arquivo, linhas, colunas, 'A'); // Escreve tabuleiro atual

    fclose(arquivo);
    printf("Jogo gravado com sucesso em '%s'.\n", nomeArquivo);
}

// Função auxiliar para procurar um separador no ficheiro
int procurar_separador(FILE *arquivo, const char *separador) {
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), arquivo)) {
        if (strstr(buffer, separador) != NULL) return 1;
    }
    return 0;
}

// Função auxiliar para ler dimensões do ficheiro
int ler_dimensoes(FILE *arquivo, int *linhas, int *colunas) {
    return fscanf(arquivo, "%d %d", linhas, colunas) == 2 && *linhas > 0 && *colunas > 0;
}

void carregarJogo(char *nomeArquivo, int *linhas, int *colunas) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro: Arquivo '%s' não encontrado.\n", nomeArquivo);
        return;
    }
    if (!ler_dimensoes(arquivo, linhas, colunas)) {
        fprintf(stderr, "Erro: Formato inválido no cabeçalho do arquivo.\n");
        fclose(arquivo);
        return;
    }
    if (tabuleiro != NULL) {
        liberarTabuleiro(*linhas);
    }
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