#ifndef HISTORICO_H
#define HISTORICO_H

#include "tabuleiro.h"

#define MAX_HISTORICO 100 // Número máximo de estados armazenados no histórico

typedef struct {
    Celula celulas[MAX_SIZE][MAX_SIZE]; // Estado das células do tabuleiro
    int linhas; // Número de linhas do tabuleiro
    int colunas; // Número de colunas do tabuleiro
} EstadoTabuleiro;

//Estrutura que representa a pilha de estados do tabuleiro
typedef struct {
    EstadoTabuleiro pilha[100]; // Array de estados
    int topo; // Índice do topo da pilha
} PilhaEstados;

void inicializarPilha(PilhaEstados *p);
int empilhar(PilhaEstados *p, int linhas, int colunas); //Guarda o estado atual do tabuleiro na pilha       
int desempilhar(PilhaEstados *p, int *linhas, int *colunas); //Restaura o estado anterior da pilha
void desfazerComando(PilhaEstados *historico, int *linhas, int *colunas);

#endif