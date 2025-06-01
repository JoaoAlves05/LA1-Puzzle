#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tabuleiro.h"
#include "historico.h"
#include "verificacoes.h"
#include "comandos.h"

int main(void){
    int linhas, colunas;
    PilhaAlteracoes historico;
    char comando[10];
    int saida = 0;

    inicializarPilha(&historico);

    printf("Digite o numero de linhas e colunas do tabuleiro: ");
    if (scanf("%d %d", &linhas, &colunas) != 2 || linhas < 1 || colunas < 1) {
        printf("Tamanho invalido.\n");
        return 1;
    }
    inicializarTabuleiro(linhas, colunas);

    printf("Digite o tabuleiro inicial (%d linhas de %d caracteres):\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            char ch;
            if (scanf(" %c", &ch) != 1) {
                printf("Erro de leitura de caractere.\n");
                return 1;
            }
            // Armazena o caractere original e atual(que neste momento são iguais)
            tabuleiro[i][j].original = tolower(ch);
            tabuleiro[i][j].atual    = tolower(ch);
        }
    }
    putchar('\n');

    while (!saida) {
        exibirTabuleiro(linhas, colunas);
        mostrar_menu();
        printf("Digite um comando: ");
        if (scanf("%s", comando) != 1) {
            saida = 1;
            continue;
        }

        if (strcmp(comando, "b") == 0) {
            processar_comando_branco(&historico, &linhas, &colunas);
        } else if (strcmp(comando, "r") == 0) {
            processar_comando_riscar(&historico, &linhas, &colunas);
        }else if (strcmp(comando, "a") == 0) {
            processar_comando_ajuda(&historico, linhas, colunas);
        } else if (strcmp(comando, "A") == 0) {
            processar_comando_ajuda_repetida(&historico, linhas, colunas);
        } else if (strcmp(comando, "R") == 0) {
            processar_comando_resolver_jogo(&historico, linhas, colunas);
        } else if (strcmp(comando, "d") == 0) {
            processar_comando_desfazer(&historico);
            printf("\nEstado após desfazer:\n");
            printf("Violações: %d\n", contarTodasAsViolacoes(linhas, colunas));
        } else if (strcmp(comando, "D") == 0) {
            processar_comando_desfazer_tudo(&historico);
            printf("\nEstado após desfazer:\n");
            printf("Violações: %d\n", contarTodasAsViolacoes(linhas, colunas));
        } else if (strcmp(comando, "v") == 0) {
            processar_comando_verificar(linhas, colunas);
        } else if (strcmp(comando, "g") == 0) {
            processar_comando_gravar(linhas, colunas);
        } else if (strcmp(comando, "l") == 0) {
            liberarPilha(&historico); // Ao carregar novo jogo, limpa histórico
            inicializarPilha(&historico); // Reinicializa a pilha
            processar_comando_carregar(&linhas, &colunas);
        } else if (strcmp(comando, "s") == 0) {
            printf("Saindo...\n");
            liberarTabuleiro(linhas);
            liberarPilha(&historico);
            saida = 1;
        } else {
            printf("Comando invalido!\n");
        }
    }

    liberarTabuleiro(linhas);
    liberarPilha(&historico);
    return 0;
}