#include <stdio.h>
#include <stdlib.h>
#include "historico.h"

// Implementação da pilha de alterações para permitir desfazer operações no tabuleiro.

// Inicializa a pilha de alterações, alocando memória inicial.
void inicializarPilha(PilhaAlteracoes *p) {
    p->topo = -1;
    p->capacidade = 10;
    p->alteracoes = malloc(p->capacidade * sizeof(AlteracaoTabuleiro));
    if (!p->alteracoes) {
        fprintf(stderr, "Erro ao alocar memória para a pilha de alterações.\n");
        p->capacidade = 0;
    }
}


// Liberta a memória associada à pilha de alterações.
void liberarPilha(PilhaAlteracoes *p) {
    if (p->alteracoes) {
        free(p->alteracoes);
        p->alteracoes = NULL;
    }
    p->topo = -1;
    p->capacidade = 0;
}

// Empilha uma nova alteração na pilha, expandindo a capacidade se necessário.
void empilhar(PilhaAlteracoes *p, int linha, int coluna, char valor_anterior, char valor_novo) {
    if (p->capacidade == 0) {
        fprintf(stderr, "Erro: pilha de alterações não inicializada.\n");
        return;
    }
    // Expande a pilha se necessário
    if (p->topo + 1 == p->capacidade) {
        int nova_capacidade = p->capacidade * 2;
        AlteracaoTabuleiro *novo = realloc(p->alteracoes, nova_capacidade * sizeof(AlteracaoTabuleiro));
        if (!novo) {
            fprintf(stderr, "Erro ao realocar memória para a pilha de alterações.\n");
            return;
        }
        p->alteracoes = novo;
        p->capacidade = nova_capacidade;
    }
    p->topo++;
    p->alteracoes[p->topo].linha = linha;
    p->alteracoes[p->topo].coluna = coluna;
    p->alteracoes[p->topo].valor_anterior = valor_anterior;
    p->alteracoes[p->topo].valor_novo = valor_novo;
}


// Desfaz a última alteração feita no tabuleiro, restaurando o valor anterior. Retorna 1 se conseguiu desfazer, 0 caso contrário.
int desfazer(PilhaAlteracoes *p) {
    if (p->capacidade == 0 || !p->alteracoes) {
        fprintf(stderr, "Erro: pilha de alterações não inicializada.\n");
        return 0;
    }
    if (p->topo < 0) {
        fprintf(stderr, "Nenhum comando para desfazer.\n");
        return 0;
    }
    AlteracaoTabuleiro alt = p->alteracoes[p->topo--];
    tabuleiro[alt.linha][alt.coluna].atual = alt.valor_anterior;
    return 1;
}

//Desfaz todas as alterações feitas, restaurando o tabuleiro ao estado inicial. Retorna 1 se conseguiu desfazer, 0 caso contrário.
int desfazer_tudo(PilhaAlteracoes *p) {
    if (p->capacidade == 0 || !p->alteracoes) {
        fprintf(stderr, "Erro: pilha de alterações não inicializada.\n");
        return 0;
    }
    if (p->topo < 0) {
        fprintf(stderr, "Nenhum comando para desfazer.\n");
        return 0;
    }
    while (p->topo >= 0) {
        desfazer(p);
    }
    return 1;
}