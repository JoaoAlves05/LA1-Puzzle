#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tabuleiro.h"
#include "historico.h"
#include "verificacoes.h"
#include "comandos.h"

// Mostra o menu de comandos disponíveis ao utilizador
void mostrar_menu(int linhas, int colunas) {
    char ultima_col = 'a' + colunas - 1;
    int ultima_linha = linhas;
    printf(ANSI_BOLD ANSI_CYAN "=== MENU DE JOGO ===\n" ANSI_RESET);
    printf(ANSI_YELLOW "Comandos disponíveis:\n" ANSI_RESET);
    printf(ANSI_BOLD " b <coluna><linha>" ANSI_RESET " - Pintar casa de branco   (ex: b a1-%c%d)\n", ultima_col, ultima_linha);
    printf(ANSI_BOLD " r <coluna><linha>" ANSI_RESET " - Riscar casa            (ex: r a1-%c%d)\n", ultima_col, ultima_linha);
    printf(ANSI_BOLD " a" ANSI_RESET " - Aplicar regras de ajuda uma vez\n");
    printf(ANSI_BOLD " A" ANSI_RESET " - Aplicar ajuda até não haver alterações\n");
    printf(ANSI_BOLD " R" ANSI_RESET " - Resolver o jogo automaticamente\n");
    printf(ANSI_BOLD " g <arquivo>" ANSI_RESET " - Gravar estado atual do jogo\n");
    printf(ANSI_BOLD " l <arquivo>" ANSI_RESET " - Carregar jogo de arquivo\n");
    printf(ANSI_BOLD " d" ANSI_RESET " - Desfazer última alteração\n");
    printf(ANSI_BOLD " D" ANSI_RESET " - Desfazer todas as alterações\n");
    printf(ANSI_BOLD " v" ANSI_RESET " - Verificar violações atuais\n");
    printf(ANSI_BOLD " s" ANSI_RESET " - Sair\n");
    printf(ANSI_YELLOW "Dica: Use 'v' para verificar o estado do tabuleiro.\n" ANSI_RESET);
}

// Processa o comando para pintar uma casa de branco
void processar_comando_branco(PilhaAlteracoes *historico, int *linhas, int *colunas) {
    char coord[5];
    printf("Coordenada: ");
    if (scanf("%4s", coord) != 1) {
        printf(ANSI_RED "Erro: leitura inválida da coordenada!\n" ANSI_RESET);
        while (getchar() != '\n');
        return;
    }
    int l, c;
    if (!input_coordenada(coord, &l, &c, *linhas, *colunas)) {
        printf(ANSI_RED "Coordenada inválida!\n" ANSI_RESET);
        return;
    }
    char valor_ant = tabuleiro[l][c].atual;
    if (pintarDeBranco(l, c)) {
        empilhar(historico, l, c, valor_ant, tabuleiro[l][c].atual);
        printf(ANSI_GREEN "Casa pintada de branco. Violações: %d\n" ANSI_RESET, 
              contarTodasAsViolacoes(*linhas, *colunas));
    } else {
        printf(ANSI_YELLOW "Não foi possível pintar a casa (já está branca ou riscada).\n" ANSI_RESET);
    }
}

// Processa o comando para riscar uma casa
void processar_comando_riscar(PilhaAlteracoes *historico, int *linhas, int *colunas) {
    char coord[5];
    printf("Coordenada: ");
    if (scanf("%4s", coord) != 1) {
        printf(ANSI_RED "Erro: leitura inválida da coordenada!\n" ANSI_RESET);
        while (getchar() != '\n');
        return;
    }
    int l, c;
    if (!input_coordenada(coord, &l, &c, *linhas, *colunas)) {
        printf(ANSI_RED "Coordenada inválida!\n" ANSI_RESET);
        return;
    }
    char valor_ant = tabuleiro[l][c].atual;
    if (riscarCasa(l, c)) {
        empilhar(historico, l, c, valor_ant, '#');
        printf(ANSI_GREEN "Casa riscada. Violações: %d\n" ANSI_RESET, 
              contarTodasAsViolacoes(*linhas, *colunas));
    } else {
        printf(ANSI_YELLOW "Não foi possível riscar a casa (já está branca ou riscada).\n" ANSI_RESET);
    }
}

// Processa o comando para gravar o estado do jogo num ficheiro
void processar_comando_gravar(int linhas, int colunas) {
    char nome[50];
    printf("Nome do arquivo para gravar: ");
    if (scanf("%49s", nome) != 1) {
        printf(ANSI_RED "Erro: nome do arquivo inválido!\n" ANSI_RESET);
        while (getchar() != '\n');
        return;
    }
    gravarJogo(nome, linhas, colunas);
}

// Processa o comando para carregar o estado do jogo de um ficheiro
void processar_comando_carregar(int *linhas, int *colunas) {
    char nome[50];
    printf("Nome do arquivo para carregar: ");
    if (scanf("%49s", nome) != 1) {
        printf(ANSI_RED "Erro: nome do arquivo inválido!\n" ANSI_RESET);
        while (getchar() != '\n');
        return;
    }
    carregarJogo(nome, linhas, colunas);
}

// Processa o comando para desfazer a última alteração
void processar_comando_desfazer(PilhaAlteracoes *historico) {
    if (desfazer(historico)) {
        printf(ANSI_GREEN "Última alteração desfeita!\n" ANSI_RESET);
    } else {
        printf(ANSI_YELLOW "Nada para desfazer!\n" ANSI_RESET);
    }
}

// Processa o comando para desfazer todas as alterações
void processar_comando_desfazer_tudo(PilhaAlteracoes *historico) {
    if (desfazer_tudo(historico)) {
        printf(ANSI_GREEN "Todas as alterações desfeitas!\n" ANSI_RESET);
    } else {
        printf(ANSI_YELLOW "Nada para desfazer!\n" ANSI_RESET);
    }
}

// Processa o comando para verificar violações no tabuleiro
void processar_comando_verificar(int linhas, int colunas) {
    int violacoes = contarTodasAsViolacoes(linhas, colunas);
    if (violacoes == 0) {
        printf(ANSI_GREEN "Tabuleiro válido! Sem violações.\n" ANSI_RESET);
    } else {
        printf(ANSI_RED "Violações encontradas:\n" ANSI_RESET);
        printf(" - Duplicados: %d\n", contarDuplicados(linhas, colunas));
        printf(" - Vizinhos inválidos: %d\n", contarVizinhos(linhas, colunas));
        printf(" - Conectividade: %d\n", contarConectividade(linhas, colunas));
        printf(ANSI_RED "Total de violações: %d\n" ANSI_RESET, violacoes);
    }
}

// Processa o comando para aplicar as regras de ajuda uma vez
void processar_comando_ajuda(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf(ANSI_CYAN "\nAplicando regras de ajuda (uma passagem)...\n" ANSI_RESET);
    int alteracoes = aplicar_todas_regras(linhas, colunas, historico);
    if (alteracoes > 0) {
        printf(ANSI_GREEN "Realizadas %d alterações.\n" ANSI_RESET, alteracoes);
        processar_comando_verificar(linhas, colunas);
    } else {
        printf(ANSI_YELLOW "Nenhuma alteração possível com as regras atuais.\n" ANSI_RESET);
    }
}

// Processa o comando para aplicar as regras de ajuda repetidamente até não haver mais alterações
void processar_comando_ajuda_repetida(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf(ANSI_CYAN "\nAplicando ajuda repetidamente...\n" ANSI_RESET);
    int total = ajuda_repetida(linhas, colunas, historico);
    printf(ANSI_GREEN "Total de %d alterações aplicando todas as regras até esgotar.\n" ANSI_RESET, total);
    processar_comando_verificar(linhas, colunas);
}

// Processa o comando para resolver o puzzle automaticamente (apenas backtracking)
void processar_comando_resolver(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf(ANSI_CYAN "\nIniciando resolução...\n" ANSI_RESET);
    PilhaAlteracoes temp_hist;
    inicializarPilha(&temp_hist);
    int resultado = resolver_jogo_backtrack(linhas, colunas, &temp_hist);
    for (int i = 0; i <= temp_hist.topo; i++) {
        AlteracaoTabuleiro alt = temp_hist.alteracoes[i];
        empilhar(historico, alt.linha, alt.coluna, alt.valor_anterior, alt.valor_novo);
    }
    if (resultado) {
        printf(ANSI_GREEN "Solução completa encontrada!\n" ANSI_RESET);
    } else {
        printf(ANSI_YELLOW "Solução parcial encontrada. Verifique violações restantes.\n" ANSI_RESET);
    }
    exibirTabuleiro(linhas, colunas);
    processar_comando_verificar(linhas, colunas);
    liberarPilha(&temp_hist);
}

// Processa o comando para resolver o puzzle, aplicando ajudas lógicas e depois backtracking
void processar_comando_resolver_jogo(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf(ANSI_CYAN "\nIniciando resolução automática...\n" ANSI_RESET);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j].atual = tabuleiro[i][j].original;
        }
    }
    PilhaAlteracoes temp_hist;
    inicializarPilha(&temp_hist);
    int total_alteracoes = ajuda_repetida(linhas, colunas, &temp_hist);
    printf(ANSI_GREEN "Total de %d alterações aplicando regras até esgotar.\n" ANSI_RESET, total_alteracoes);
    int resultado = resolver_jogo_backtrack(linhas, colunas, &temp_hist);
    for (int i = 0; i <= temp_hist.topo; i++) {
        AlteracaoTabuleiro alt = temp_hist.alteracoes[i];
        empilhar(historico, alt.linha, alt.coluna, alt.valor_anterior, alt.valor_novo);
    }
    if (resultado) {
        printf(ANSI_GREEN "Solução completa encontrada!\n" ANSI_RESET);
    } else {
        printf(ANSI_YELLOW "Solução parcial encontrada após backtracking. Verifique violações restantes.\n" ANSI_RESET);
    }
    int violacoes = contarTodasAsViolacoes(linhas, colunas);
    if (violacoes == 0) {
        printf(ANSI_GREEN "Tabuleiro válido! Sem violações.\n" ANSI_RESET);
    } else {
        printf(ANSI_RED "Total de violações: %d\n" ANSI_RESET, violacoes);
    }
    liberarPilha(&temp_hist);
}
