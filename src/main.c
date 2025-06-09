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

int menu_inicial() {
    int opcao = 0;
    printf(ANSI_BOLD ANSI_CYAN "\n======= MENU INICIAL =======\n" ANSI_RESET);
    printf("1 - Novo jogo (inserir tabuleiro manualmente)\n");
    printf("2 - Carregar jogo de ficheiro\n");
    printf("0 - Sair\n");
    printf("============================\n");
    while (1) {
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            printf(ANSI_RED "Opção inválida!\n" ANSI_RESET);
            continue;
        }
        if (opcao == 0 || opcao == 1 || opcao == 2) break;
        printf(ANSI_RED "Opção inválida!\n" ANSI_RESET);
    }
    while (getchar() != '\n');
    return opcao;
}

int inserir_tabuleiro_manual(int *linhas, int *colunas) {
    printf(ANSI_BOLD "\nInsira o tabuleiro inicial (%d linhas de %d caracteres cada).\n" ANSI_RESET, *linhas, *colunas);
    printf(ANSI_CYAN "Use letras minúsculas para casas, '#' para casas riscadas.\n" ANSI_RESET);
    printf(ANSI_CYAN "Exemplo de linha: abcd#\n" ANSI_RESET);
    printf(ANSI_YELLOW "Pode inserir linha a linha, ou colar todas as linhas de uma vez (pressione ENTER após cada linha).\n" ANSI_RESET);
    printf(ANSI_YELLOW "Digite 'corrigir N' para corrigir a linha N (ex: corrigir 3).\n" ANSI_RESET);
    printf(ANSI_YELLOW "Digite 'cancelar' a qualquer momento para voltar ao menu inicial.\n\n" ANSI_RESET);

    char linhas_buffer[32][128];
    int linhas_preenchidas = 0;
    int mostrar_tabuleiro = 0;

    while (linhas_preenchidas < *linhas) {
        printf(ANSI_BOLD ANSI_CYAN "\nLinha %d: " ANSI_RESET, linhas_preenchidas + 1);
        char buffer[128];
        if (!ler_linha(buffer, sizeof(buffer))) {
            printf(ANSI_RED "Erro de leitura!\n" ANSI_RESET);
            continue;
        }
        if (strcmp(buffer, "cancelar") == 0) {
            printf(ANSI_YELLOW "Inserção cancelada. Voltando ao menu inicial.\n" ANSI_RESET);
            return 0;
        }
        if (strncmp(buffer, "corrigir ", 9) == 0) {
            int n = atoi(buffer + 9);
            if (n >= 1 && n <= linhas_preenchidas) {
                printf(ANSI_BOLD ANSI_CYAN "Nova linha %d: " ANSI_RESET, n);
                if (!ler_linha(linhas_buffer[n-1], sizeof(linhas_buffer[n-1]))) {
                    printf(ANSI_RED "Erro de leitura!\n" ANSI_RESET);
                }
                mostrar_tabuleiro = 1;
                continue;
            } else {
                printf(ANSI_YELLOW "Número de linha inválido para corrigir.\n" ANSI_RESET);
                continue;
            }
        }
        // Remove espaços entre letras ao colar/tabular
        char temp[128];
        int k = 0;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isspace((unsigned char)buffer[i])) {
                temp[k++] = buffer[i];
            }
        }
        temp[k] = '\0';

        // Se o utilizador colou várias linhas, processa-as todas
        char *ptr = temp;
        int linhas_antes = linhas_preenchidas;
        while (linhas_preenchidas < *linhas && *ptr) {
            char linha[128];
            int l = 0;
            while (*ptr && *ptr != '\n' && *ptr != '\r' && l < *colunas) {
                linha[l++] = *ptr++;
            }
            linha[l] = '\0';
            while (*ptr == '\n' || *ptr == '\r') ptr++;
            if (strcmp(linha, "cancelar") == 0) {
                printf(ANSI_YELLOW "Inserção cancelada. Voltando ao menu inicial.\n" ANSI_RESET);
                return 0;
            }
            if ((int)strlen(linha) != *colunas) {
                printf(ANSI_YELLOW "A linha deve ter exatamente %d caracteres!\n" ANSI_RESET, *colunas);
                break;
            }
            int valido = 1;
            for (int j = 0; j < *colunas; j++) {
                char ch = linha[j];
                if (!(isalpha(ch) || ch == '#')) {
                    valido = 0;
                    break;
                }
            }
            if (!valido) {
                printf(ANSI_YELLOW "Só são permitidas letras ou '#'.\n" ANSI_RESET);
                break;
            }
            strcpy(linhas_buffer[linhas_preenchidas], linha);
            linhas_preenchidas++;
        }
        // Só mostra o tabuleiro parcial uma vez após colagem de várias linhas
        if (linhas_preenchidas > linhas_antes || mostrar_tabuleiro) {
            printf(ANSI_GREEN "\nTabuleiro parcial:\n" ANSI_RESET);
            for (int i = 0; i < linhas_preenchidas; i++) {
                printf(ANSI_BOLD " %2d: %s\n" ANSI_RESET, i + 1, linhas_buffer[i]);
            }
            mostrar_tabuleiro = 0;
        }
    }

    // Permitir correção antes de confirmar
    while (1) {
        printf(ANSI_GREEN "\nTabuleiro inserido:\n" ANSI_RESET);
        for (int i = 0; i < *linhas; i++) {
            printf(ANSI_BOLD " %2d: %s\n" ANSI_RESET, i + 1, linhas_buffer[i]);
        }
        printf(ANSI_YELLOW "\nDigite 'corrigir N' para editar uma linha, 'ok' para confirmar, ou 'cancelar' para voltar ao menu inicial.\n" ANSI_RESET);
        char buffer[128];
        printf(ANSI_BOLD "Comando: " ANSI_RESET);
        if (!ler_linha(buffer, sizeof(buffer))) continue;
        if (strcmp(buffer, "ok") == 0) break;
        if (strcmp(buffer, "cancelar") == 0) {
            printf(ANSI_YELLOW "Inserção cancelada. Voltando ao menu inicial.\n" ANSI_RESET);
            return 0;
        }
        if (strncmp(buffer, "corrigir ", 9) == 0) {
            int n = atoi(buffer + 9);
            if (n >= 1 && n <= *linhas) {
                printf(ANSI_CYAN "Nova linha %d: " ANSI_RESET, n);
                if (!ler_linha(linhas_buffer[n-1], sizeof(linhas_buffer[n-1]))) {
                    printf(ANSI_RED "Erro de leitura!\n" ANSI_RESET);
                    continue;
                }
                // Remove espaços ao corrigir
                char temp[128];
                int k = 0;
                for (int i = 0; linhas_buffer[n-1][i] != '\0'; i++) {
                    if (!isspace((unsigned char)linhas_buffer[n-1][i])) {
                        temp[k++] = linhas_buffer[n-1][i];
                    }
                }
                temp[k] = '\0';
                strcpy(linhas_buffer[n-1], temp);

                if ((int)strlen(linhas_buffer[n-1]) != *colunas) {
                    printf(ANSI_YELLOW "A linha deve ter exatamente %d caracteres!\n" ANSI_RESET, *colunas);
                    continue;
                }
                int valido = 1;
                for (int j = 0; j < *colunas; j++) {
                    char ch = linhas_buffer[n-1][j];
                    if (!(isalpha(ch) || ch == '#')) {
                        valido = 0;
                        break;
                    }
                }
                if (!valido) {
                    printf(ANSI_YELLOW "Só são permitidas letras ou '#'.\n" ANSI_RESET);
                    continue;
                }
            } else {
                printf(ANSI_YELLOW "Número de linha inválido para corrigir.\n" ANSI_RESET);
            }
            continue;
        }
        printf(ANSI_YELLOW "Comando não reconhecido.\n" ANSI_RESET);
    }

    // Copiar para o tabuleiro global
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            tabuleiro[i][j].original = tolower(linhas_buffer[i][j]);
            tabuleiro[i][j].atual    = tolower(linhas_buffer[i][j]);
        }
    }
    printf(ANSI_GREEN "\nTabuleiro inicial inserido com sucesso!\n" ANSI_RESET);
    // Não pede ENTER, começa logo o jogo
    return 1;
}

// Adicione os protótipos das funções auxiliares usadas para leitura de ficheiros
int ler_dimensoes(FILE *arquivo, int *linhas, int *colunas);
int procurar_separador(FILE *arquivo, char *separador);
int lerTabuleiro(FILE *arquivo, int linhas, int colunas, char tipo);

// Altere a função carregarJogo para retornar sucesso/erro
int carregarJogoComRetorno(char *nomeArquivo, int *linhas, int *colunas) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro: Arquivo '%s' não encontrado.\n", nomeArquivo);
        return 0;
    }
    int novas_linhas, novas_colunas;
    if (!ler_dimensoes(arquivo, &novas_linhas, &novas_colunas)) {
        fprintf(stderr, "Erro: Formato inválido no cabeçalho do arquivo.\n");
        fclose(arquivo);
        return 0;
    }
    if (tabuleiro != NULL) {
        liberarTabuleiro(tabuleiro_linhas);
    }
    *linhas = novas_linhas;
    *colunas = novas_colunas;
    inicializarTabuleiro(*linhas, *colunas);
    if (!procurar_separador(arquivo, "ORIGINAL")) {
        fprintf(stderr, "Erro: Separador 'ORIGINAL' não encontrado.\n");
        fclose(arquivo);
        return 0;
    }
    if (!lerTabuleiro(arquivo, *linhas, *colunas, 'O')) {
        fclose(arquivo);
        return 0;
    }
    if (!procurar_separador(arquivo, "ATUAL")) {
        fprintf(stderr, "Erro: Separador 'ATUAL' não encontrado.\n");
        fclose(arquivo);
        return 0;
    }
    if (!lerTabuleiro(arquivo, *linhas, *colunas, 'A')) {
        fclose(arquivo);
        return 0;
    }
    fclose(arquivo);
    printf("Jogo carregado com sucesso de '%s'.\n", nomeArquivo);
    return 1;
}

int main(void){
    int linhas = 0, colunas = 0;
    PilhaAlteracoes historico;
    char comando[16];
    int saida = 0;

    // Imprime o banner e mensagem de boas-vindas apenas uma vez no início
    clear_screen();
    ascii_art();
    printf(ANSI_BOLD ANSI_GREEN "Bem-vindo ao Puzzle das Letras!\n" ANSI_RESET);
    printf("Desenvolvido por um verdadeiro expert em C!\n\n");

    int opcao;
    do {
        // Remova estas linhas do ciclo:
        // clear_screen();
        // ascii_art();
        // printf(ANSI_BOLD ANSI_GREEN "Bem-vindo ao Puzzle das Letras!\n" ANSI_RESET);
        // printf("Desenvolvido por um verdadeiro expert em C!\n\n");

        opcao = menu_inicial();
        if (opcao == 0) {
            printf(ANSI_BOLD ANSI_CYAN "Até breve!\n" ANSI_RESET);
            return 0;
        }

        inicializarPilha(&historico);

        if (opcao == 2) {
            // Carregar de ficheiro
            char nome[128];
            printf("Nome do ficheiro para carregar (apenas o nome, sem 'saved/'): ");
            if (scanf("%127s", nome) != 1) {
                printf(ANSI_RED "Erro ao ler nome do ficheiro!\n" ANSI_RESET);
                while (getchar() != '\n');
                liberarPilha(&historico);
                pause_enter("Prima ENTER para voltar ao menu inicial...");
                continue;
            }
            char caminho[256];
            snprintf(caminho, sizeof(caminho), "saved/%s", nome);
            FILE *arquivo = fopen(caminho, "r");
            if (!arquivo) {
                printf(ANSI_RED "Erro: Arquivo '%s' não encontrado.\n" ANSI_RESET, caminho);
                liberarPilha(&historico);
                pause_enter("Prima ENTER para voltar ao menu inicial...");
                continue;
            }
            int novas_linhas, novas_colunas;
            if (fscanf(arquivo, "%d %d", &novas_linhas, &novas_colunas) != 2 || novas_linhas <= 0 || novas_colunas <= 0) {
                printf(ANSI_RED "Erro: Formato inválido no cabeçalho do arquivo.\n" ANSI_RESET);
                fclose(arquivo);
                liberarPilha(&historico);
                pause_enter("Prima ENTER para voltar ao menu inicial...");
                continue;
            }
            if (tabuleiro != NULL) liberarTabuleiro(tabuleiro_linhas);
            linhas = novas_linhas;
            colunas = novas_colunas;
            inicializarTabuleiro(linhas, colunas);

            // Procura separador "ORIGINAL"
            char buffer[100];
            int encontrou = 0;
            while (fgets(buffer, sizeof(buffer), arquivo)) {
                if (strstr(buffer, "ORIGINAL")) { encontrou = 1; break; }
            }
            if (!encontrou) {
                printf(ANSI_RED "Erro: Separador 'ORIGINAL' não encontrado.\n" ANSI_RESET);
                fclose(arquivo);
                liberarTabuleiro(linhas);
                liberarPilha(&historico);
                pause_enter("Prima ENTER para voltar ao menu inicial...");
                continue;
            }
            // Lê tabuleiro original
            int ok = 1;
            for (int i = 0; i < linhas && ok; i++) {
                for (int j = 0; j < colunas && ok; j++) {
                    char c;
                    if (fscanf(arquivo, " %c", &c) != 1 || !(isalpha(c) || c == '#')) {
                        printf(ANSI_RED "Erro: Falha ao ler caractere na linha %d, coluna %d (Original).\n" ANSI_RESET, i + 1, j + 1);
                        ok = 0;
                    } else {
                        tabuleiro[i][j].original = c;
                    }
                }
            }
            if (!ok) {
                fclose(arquivo);
                liberarTabuleiro(linhas);
                liberarPilha(&historico);
                pause_enter("Prima ENTER para voltar ao menu inicial...");
                continue;
            }
            // Procura separador "ATUAL"
            encontrou = 0;
            while (fgets(buffer, sizeof(buffer), arquivo)) {
                if (strstr(buffer, "ATUAL")) { encontrou = 1; break; }
            }
            if (!encontrou) {
                printf(ANSI_RED "Erro: Separador 'ATUAL' não encontrado.\n" ANSI_RESET);
                fclose(arquivo);
                liberarTabuleiro(linhas);
                liberarPilha(&historico);
                pause_enter("Prima ENTER para voltar ao menu inicial...");
                continue;
            }
            // Lê tabuleiro atual
            for (int i = 0; i < linhas && ok; i++) {
                for (int j = 0; j < colunas && ok; j++) {
                    char c;
                    if (fscanf(arquivo, " %c", &c) != 1 || !(isalpha(c) || c == '#')) {
                        printf(ANSI_RED "Erro: Falha ao ler caractere na linha %d, coluna %d (Atual).\n" ANSI_RESET, i + 1, j + 1);
                        ok = 0;
                    } else {
                        tabuleiro[i][j].atual = c;
                    }
                }
            }
            fclose(arquivo);
            if (!ok) {
                liberarTabuleiro(linhas);
                liberarPilha(&historico);
                pause_enter("Prima ENTER para voltar ao menu inicial...");
                continue;
            }
            printf(ANSI_GREEN "Jogo carregado com sucesso de '%s'.\n" ANSI_RESET, caminho);
            break;
        } else if (opcao == 1) {
            // Novo jogo: pedir linhas/colunas e tabuleiro
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

            inicializarTabuleiro(linhas, colunas);

            if (inserir_tabuleiro_manual(&linhas, &colunas)) {
                clear_screen();
                break;
            } else {
                liberarTabuleiro(linhas);
                liberarPilha(&historico);
                // Volta ao menu inicial
            }
        }
    } while (1);

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