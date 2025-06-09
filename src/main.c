#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tabuleiro.h"
#include "historico.h"
#include "verificacoes.h"
#include "comandos.h"

void clear_screen() {
    printf("\033[2J\033[H");
}

void pause_enter(const char *msg) {
    printf("%s", msg ? msg : "Prima ENTER para continuar...");
    while (getchar() != '\n');
}

int ler_linha(char *buffer, int max) {
    if (fgets(buffer, max, stdin) == NULL) return 0;
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
    return 1;
}

void ascii_art() {
    printf(ANSI_BOLD ANSI_CYAN);
    printf("========================================\n");
    printf("  _____  _    _  _______  _______ __     ______ \n");
    printf(" | __ \\ | |  | ||___   / |___   /|  |   |  ____|\n");
    printf(" | |__) | |  | |   /  /     /  / |  |   | |__   \n");
    printf(" |  ___/| |  | |  /  /     /  /  |  |   |  __|  \n");
    printf(" | |    | |__| | /  /___  /  /__ |  |___| |____ \n");
    printf(" |_|     \\____/ |_______||______||______|______|\n");
    printf("                                              \n");
    printf("        D A S   L E T R A S                  \n");
    printf("========================================\n");
    printf(ANSI_RESET);
}

int main(void){
    int linhas = 0, colunas = 0;
    PilhaAlteracoes historico;
    char comando[16];
    int saida = 0;

    clear_screen();
    ascii_art();
    printf(ANSI_BOLD ANSI_GREEN "Bem-vindo ao Puzzle das Letras!\n" ANSI_RESET);
    printf("Desenvolvido por um verdadeiro expert em C!\n\n");

    // Leitura robusta do número de linhas e colunas
    while (1) {
        printf(ANSI_BOLD "Quantas linhas e colunas terá o tabuleiro? (ex: 5 7)\n" ANSI_RESET);
        printf("Linhas (1-26): ");
        if (scanf("%d", &linhas) != 1 || linhas < 1 || linhas > 26) {
            printf(ANSI_RED "Valor inválido para linhas!\n" ANSI_RESET);
            while (getchar() != '\n');
            continue;
        }
        printf("Colunas (1-26): ");
        if (scanf("%d", &colunas) != 1 || colunas < 1 || colunas > 26) {
            printf(ANSI_RED "Valor inválido para colunas!\n" ANSI_RESET);
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        break;
    }

    inicializarPilha(&historico);
    inicializarTabuleiro(linhas, colunas);

    // Inserção interativa do tabuleiro
    printf(ANSI_BOLD "\nInsira o tabuleiro inicial (%d linhas de %d caracteres cada).\n" ANSI_RESET, linhas, colunas);
    printf("Use letras minúsculas para casas, '#' para casas riscadas.\n");
    printf("Exemplo de linha: abcd#\n\n");

    char buffer[128];
    for (int i = 0; i < linhas; i++) {
        while (1) {
            printf(ANSI_CYAN "Linha %d: " ANSI_RESET, i + 1);
            if (!ler_linha(buffer, sizeof(buffer))) {
                printf(ANSI_RED "Erro de leitura!\n" ANSI_RESET);
                continue;
            }
            if ((int)strlen(buffer) != colunas) {
                printf(ANSI_YELLOW "A linha deve ter exatamente %d caracteres!\n" ANSI_RESET, colunas);
                continue;
            }
            int valido = 1;
            for (int j = 0; j < colunas; j++) {
                char ch = buffer[j];
                if (!(isalpha(ch) || ch == '#')) {
                    valido = 0;
                    break;
                }
                tabuleiro[i][j].original = tolower(ch);
                tabuleiro[i][j].atual    = tolower(ch);
            }
            if (!valido) {
                printf(ANSI_YELLOW "Só são permitidas letras ou '#'.\n" ANSI_RESET);
                continue;
            }
            break;
        }
        exibirTabuleiro(i+1, colunas);
    }
    printf(ANSI_GREEN "\nTabuleiro inicial inserido com sucesso!\n" ANSI_RESET);
    pause_enter("Prima ENTER para começar o jogo...");
    clear_screen();

    // Loop principal do jogo
    while (!saida) {
        exibirTabuleiro(linhas, colunas);
        mostrar_menu(linhas, colunas);
        printf("Digite um comando: ");
        if (scanf("%15s", comando) != 1) {
            saida = 1;
        } else {
            if (strcmp(comando, "b") == 0) {
                processar_comando_branco(&historico, &linhas, &colunas);
            } else if (strcmp(comando, "r") == 0) {
                processar_comando_riscar(&historico, &linhas, &colunas);
            } else if (strcmp(comando, "a") == 0) {
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
                liberarPilha(&historico);
                inicializarPilha(&historico);
                processar_comando_carregar(&linhas, &colunas);
            } else if (strcmp(comando, "s") == 0) {
                printf(ANSI_BOLD ANSI_CYAN "Obrigado por jogar! Até breve.\n" ANSI_RESET);
                liberarTabuleiro(linhas);
                liberarPilha(&historico);
                saida = 1;
            } else {
                printf(ANSI_RED "Comando invalido!\n" ANSI_RESET);
            }
        }
        while (getchar() != '\n'); // Limpa buffer
    }

    liberarTabuleiro(linhas);
    liberarPilha(&historico);
    return 0;
}