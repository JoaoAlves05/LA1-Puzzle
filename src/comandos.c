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
    printf("\nComandos disponíveis:\n");
    printf(" b <coluna><linha> - Pintar casa de branco (ex: b a1-%c%d)\n", ultima_col, ultima_linha);
    printf(" r <coluna><linha> - Riscar casa (ex: r a1-%c%d)\n", ultima_col, ultima_linha);
    printf(" a - Aplicar regras de ajuda uma vez\n");
    printf(" A - Aplicar ajuda até não haver mais alterações\n");
    printf(" R - Resolver o jogo automaticamente\n");
    printf(" g <arquivo> - Gravar estado atual do jogo\n");
    printf(" l <arquivo> - Carregar jogo de arquivo\n");
    printf(" d - Desfazer última alteração\n");
    printf(" D - Desfazer todas as alterações\n");
    printf(" v - Verificar violações atuais\n");
    printf(" s - Sair\n");
}

// Processa o comando para pintar uma casa de branco
void processar_comando_branco(PilhaAlteracoes *historico, int *linhas, int *colunas) {
    char coord[5];
    printf("Coordenada: ");
    if (scanf("%4s", coord) != 1) {
        printf("Erro na leitura da coordenada!\n");
        return;
    }

    int l, c;
    if (!input_coordenada(coord, &l, &c, *linhas, *colunas)) {
        printf("Coordenada inválida!\n");
        return;
    }

    char valor_ant = tabuleiro[l][c].atual;
    if (pintarDeBranco(l, c)) {
        empilhar(historico, l, c, valor_ant, tabuleiro[l][c].atual);
        printf("Casa pintada de branco. Violações: %d\n", 
              contarTodasAsViolacoes(*linhas, *colunas));
    } else {
        printf("Não foi possível pintar a casa!\n");
    }
}

// Processa o comando para riscar uma casa
void processar_comando_riscar(PilhaAlteracoes *historico, int *linhas, int *colunas) {
    char coord[5];
    printf("Coordenada: ");
    if (scanf("%4s", coord) != 1) {
        printf("Erro na leitura da coordenada!\n");
        return;
    }

    int l, c;
    if (!input_coordenada(coord, &l, &c, *linhas, *colunas)) {
        printf("Coordenada inválida!\n");
        return;
    }

    char valor_ant = tabuleiro[l][c].atual;
    if (riscarCasa(l, c)) {
        empilhar(historico, l, c, valor_ant, '#');
        printf("Casa riscada. Violações: %d\n", 
              contarTodasAsViolacoes(*linhas, *colunas));
    } else {
        printf("Não foi possível riscar a casa!\n");
    }
}

// Processa o comando para gravar o estado do jogo num ficheiro
void processar_comando_gravar(int linhas, int colunas) {
    char nome[50];
    printf("Nome do arquivo para gravar: ");
    if (scanf("%49s", nome) != 1) {
        printf("Erro no nome do arquivo!\n");
        return;
    }
    gravarJogo(nome, linhas, colunas);
    printf("Jogo gravado com sucesso!\n");
}

// Processa o comando para carregar o estado do jogo de um ficheiro
void processar_comando_carregar(int *linhas, int *colunas) {
    char nome[50];
    printf("Nome do arquivo para carregar: ");
    if (scanf("%49s", nome) != 1) {
        printf("Erro no nome do arquivo!\n");
        return;
    }
    carregarJogo(nome, linhas, colunas);
    printf("Jogo carregado com sucesso!\n");
}

// Processa o comando para desfazer a última alteração
void processar_comando_desfazer(PilhaAlteracoes *historico) {
    if (desfazer(historico)) {
        printf("Última alteração desfeita!\n");
    } else {
        printf("Nada para desfazer!\n");
    }
}

// Processa o comando para desfazer todas as alterações
void processar_comando_desfazer_tudo(PilhaAlteracoes *historico) {
    desfazer_tudo(historico);
    printf("Todas as alterações desfeitas!\n");
}

// Processa o comando para verificar violações no tabuleiro
void processar_comando_verificar(int linhas, int colunas) {
    int violacoes = contarTodasAsViolacoes(linhas, colunas);
    if (violacoes == 0) {
        printf("Tabuleiro válido! Sem violações.\n");
    } else {
        printf("Violações encontradas:\n");
        printf(" - Duplicados: %d\n", contarDuplicados(linhas, colunas));
        printf(" - Vizinhos inválidos: %d\n", contarVizinhos(linhas, colunas));
        printf(" - Conectividade: %d\n", contarConectividade(linhas, colunas));
        printf("Total de violações: %d\n", violacoes);
    }
}

// Processa o comando para aplicar as regras de ajuda uma vez
void processar_comando_ajuda(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf("\nAplicando regras de ajuda (uma passagem)...\n");
    int alteracoes = aplicar_todas_regras(linhas, colunas, historico);
    if (alteracoes > 0) {
        printf("Realizadas %d alterações:\n", alteracoes);
        processar_comando_verificar(linhas, colunas);
    } else {
        printf("Nenhuma alteração possível com as regras atuais.\n");
    }
}

// Processa o comando para aplicar as regras de ajuda repetidamente até não haver mais alterações
void processar_comando_ajuda_repetida(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf("\nAplicando ajuda repetidamente...\n");
    int total = ajuda_repetida(linhas, colunas, historico);
    printf("\nTotal de %d alterações aplicando todas as regras até esgotar:\n", total);
    processar_comando_verificar(linhas, colunas);
}

// Processa o comando para resolver o puzzle automaticamente (apenas backtracking)
void processar_comando_resolver(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf("\nIniciando resolução...\n");
    PilhaAlteracoes temp_hist;
    inicializarPilha(&temp_hist);
    int resultado = 0;
    // Chama o resolvedor por backtracking
    resultado = resolver_jogo_backtrack(linhas, colunas, &temp_hist);
    // Transfere alterações para o histórico principal
    for (int i = 0; i <= temp_hist.topo; i++) {
        AlteracaoTabuleiro alt = temp_hist.alteracoes[i];
        empilhar(historico, alt.linha, alt.coluna, alt.valor_anterior, alt.valor_novo);
    }
    if (resultado) {
        printf("Solução completa encontrada!\n");
    } else {
        printf("Solução parcial encontrada. Verifique violações restantes.\n");
    }
    exibirTabuleiro(linhas, colunas);
    processar_comando_verificar(linhas, colunas);
    liberarPilha(&temp_hist);
}

// Processa o comando para resolver o puzzle, aplicando ajudas lógicas e depois backtracking
void processar_comando_resolver_jogo(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf("\nIniciando resolução automática...\n");

    // Repõe o estado atual do tabuleiro para o original
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j].atual = tabuleiro[i][j].original;
        }
    }

    PilhaAlteracoes temp_hist;
    inicializarPilha(&temp_hist);

    // Aplica deduções lógicas antes do backtracking
    int total_alteracoes = ajuda_repetida(linhas, colunas, &temp_hist);
    printf("Total de %d alterações aplicando regras até esgotar:\n", total_alteracoes);

    // Depois aplica o resolvedor por backtracking para completar a solução
    int resultado = resolver_jogo_backtrack(linhas, colunas, &temp_hist);

    // Transfere as alterações do histórico temporário para o principal
    for (int i = 0; i <= temp_hist.topo; i++) {
        AlteracaoTabuleiro alt = temp_hist.alteracoes[i];
        empilhar(historico, alt.linha, alt.coluna, alt.valor_anterior, alt.valor_novo);
    }

    if (resultado) {
        printf("Solução completa encontrada!\n");
    } else {
        printf("Solução parcial encontrada após backtracking. Verifique violações restantes.\n");
    }

    int violacoes = contarTodasAsViolacoes(linhas, colunas);
    if (violacoes == 0) {
        printf("Tabuleiro válido! Sem violações.\n");
    } else {
        printf("Total de violações: %d\n", violacoes);
    }

    liberarPilha(&temp_hist);
}
