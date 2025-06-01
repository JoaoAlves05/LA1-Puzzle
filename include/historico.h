#ifndef HISTORICO_H
#define HISTORICO_H

#include "tabuleiro.h"

// Estrutura que representa uma alteração feita no tabuleiro (para desfazer/refazer)
typedef struct {
    int linha, coluna;      // Posição da alteração
    char valor_anterior;    // Valor antes da alteração
    char valor_novo;        // Valor depois da alteração
} AlteracaoTabuleiro;

// Estrutura que representa uma pilha dinâmica de alterações
typedef struct {
    AlteracaoTabuleiro *alteracoes; // Array dinâmico de alterações
    int topo;                       // Índice do topo da pilha
    int capacidade;                 // Capacidade atual do array
} PilhaAlteracoes;

//Inicializa uma pilha de alterações.
void inicializarPilha(PilhaAlteracoes *p);

//Liberta a memória associada à pilha.
void liberarPilha(PilhaAlteracoes *p);

// Empilha uma nova alteração na pilha
void empilhar(PilhaAlteracoes *p, int linha, int coluna, char valor_anterior, char valor_novo);

//Desfaz a última alteração feita no tabuleiro. Retorna 1 se conseguiu desfazer, 0 caso contrário.
int desfazer(PilhaAlteracoes *p);

 // Desfaz todas as alterações feitas no tabuleiro. Retorna 1 se conseguiu desfazer, 0 caso contrário.
int desfazer_tudo(PilhaAlteracoes *p);

#endif