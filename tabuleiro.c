#include <stdio.h>
#include "tabuleiro.h"

//Verificação do input das coordenadas
int input_coordenada(const char *coordenada, int *linha, int *coluna, int max_linhas, int max_colunas) {
    // Verifica se a coordenada tem o formato correto
    if (coordenada[0] == '\0' || !isalpha(coordenada[0]) || !isdigit(coordenada[1])) {
        return 0;  // Formato inválido
    }

    *coluna = tolower(coordenada[0]) - 'a'; // Converte a letra da coordenada para índice do tabuleiro
    *linha = atoi(coordenada + 1) - 1; // Converte string para inteiro de acordo com os índices do tabuleiro

    // Verifica se está dentro dos limites
    return (*linha >= 0 && *linha < max_linhas && *coluna >= 0 && *coluna < max_colunas);
}

void exibirTabuleiro(int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j].atual);
        }
        printf("\n");
    }
}

//Função que pinta a casa de branco se já não estiver riscada ou pintada
int pintarDeBranco(int linha, int coluna) {
    if (tabuleiro[linha][coluna].atual == '#') {
        printf("Erro: Essa casa já está riscada com #.\n");
        return 0;
    }
    if (isupper(tabuleiro[linha][coluna].atual)) {
        printf("Erro: Essa casa já está pintada de branco.\n");
        return 0;
    }
    tabuleiro[linha][coluna].atual = toupper(tabuleiro[linha][coluna].original);
    return 1; // Foi pintado com sucesso
}

//Função que risca a casa se já não estiver riscada ou pintada
int riscarCasa(int linha, int coluna) {
    if (tabuleiro[linha][coluna].atual == '#') {
        printf("Erro: Essa casa já está riscada com #.\n");
        return 0;
    }
    if (isupper(tabuleiro[linha][coluna].atual)) {
        printf("Erro: Essa casa já está pintada de branco.\n");
        return 0;
    }
    tabuleiro[linha][coluna].atual = '#';
    return 1; // Foi riscada com sucesso
}

void gravarJogo(char *nomeArquivo, int linhas, int colunas) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para gravação.\n");
        return;
    }

    // Grava o número de linhas e colunas
    fprintf(arquivo, "%d %d\n", linhas, colunas); 

    // Grava o tabuleiro atual
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "%c", tabuleiro[i][j].atual);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Jogo guardado no arquivo '%s'.\n", nomeArquivo);
}

void carregarJogo(char *nomeArquivo, int *linhas, int *colunas) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    // Lê o número de linhas e colunas
    fscanf(arquivo, "%d %d", linhas, colunas);

    //Lê o estado do tabuleiro do arquivo
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            fscanf(arquivo, " %c", &tabuleiro[i][j].atual);
            tabuleiro[i][j].original = tolower(tabuleiro[i][j].atual);
        }
    }

    fclose(arquivo);
    printf("Jogo carregado do arquivo '%s'.\n", nomeArquivo);
}
