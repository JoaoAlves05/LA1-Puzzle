#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tabuleiro.h"
#include "historico.h"
#include "verificacoes.h"
#include "comandos.h"

void mostrar_menu() {
    printf("\nComandos disponíveis:\n");
    printf(" b <coord>   - Pintar casa de branco (ex: b a1)\n");
    printf(" r <coord>   - Riscar casa (ex: r b2)\n");
    printf(" a - Aplicar regras de ajuda uma vez\n");
    printf(" A - Aplicar ajuda até não haver mais alterações\n");
    printf(" R - Resolver o jogo automaticamente\n");
    printf(" g <arquivo> - Gravar estado atual do jogo\n");
    printf(" l <arquivo> - Carregar jogo de arquivo\n");
    printf(" d - Desfazer última alteração\n");
    printf(" v - Verificar violações atuais\n");
    printf(" s - Sair\n");
}

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

void processar_comando_desfazer(PilhaAlteracoes *historico) {
    if (desfazer(historico)) {
        printf("Última alteração desfeita!\n");
    } else {
        printf("Nada para desfazer!\n");
    }
}

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

void processar_comando_ajuda(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf("\nAplicando regras de ajuda...\n");
    int alteracoes = ajuda_automatica(linhas, colunas, historico);
    
    if (alteracoes > 0) {
        printf("Realizadas %d alterações:\n", alteracoes);
        exibirTabuleiro(linhas, colunas);
        processar_comando_verificar(linhas, colunas);
    } else {
        printf("Nenhuma alteração possível com as regras atuais.\n");
    }
}

void processar_comando_ajuda_repetida(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf("\nAplicando ajuda repetidamente...\n");
    int total = 0;
    int passos = 0;
    
    do {
        int alteracoes = ajuda_automatica(linhas, colunas, historico);
        if (alteracoes == 0) break;
        
        total += alteracoes;
        passos++;
        printf("Passo %d: %d alterações\n", passos, alteracoes);
    } while (1);

    printf("\nTotal de %d alterações em %d passos:\n", total, passos);
    exibirTabuleiro(linhas, colunas);
    processar_comando_verificar(linhas, colunas);
}

void processar_comando_resolver(PilhaAlteracoes *historico, int linhas, int colunas) {
    printf("\nIniciando resolução automática...\n");
    
    // Use temporary history to allow full undo if needed
    PilhaAlteracoes temp_hist;
    inicializarPilha(&temp_hist);
    
    int resultado = resolver_jogo(linhas, colunas, &temp_hist);
    
    // Transfer changes to main history
    while (temp_hist.topo >= 0) {
        AlteracaoTabuleiro alt = temp_hist.alteracoes[temp_hist.topo--];
        empilhar(historico, alt.linha, alt.coluna, alt.valor_anterior, alt.valor_novo);
    }
    
    if (resultado) {
        printf("Solução completa encontrada!\n");
    } else {
        printf("Solução parcial encontrada. Verifique violações remanescentes.\n");
    }
    
    exibirTabuleiro(linhas, colunas);
    processar_comando_verificar(linhas, colunas);
    liberarPilha(&temp_hist);
}
void processar_comando_resolver_jogo(Celula **tabuleiro, int linhas, int colunas) {
    printf("Resolvendo o jogo...\n");
    resolver_jogo(tabuleiro, linhas, colunas);
}