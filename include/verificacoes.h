#ifndef VERIFICACOES_H
#define VERIFICACOES_H
#include "historico.h"
#include "tabuleiro.h"

// Verification functions
int contarDuplicados(int linhas, int colunas);
int contarReplicas(int linhas, int colunas);
int contarVizinhos(int linhas, int colunas);
int contarConectividade(int linhas, int colunas);
int contarTodasAsViolacoes(int linhas, int colunas);

// Auto-solve functions
int riscar_duplicados(int linhas, int colunas, PilhaAlteracoes *hist);
int pintar_vizinhos_riscados(int linhas, int colunas, PilhaAlteracoes *hist);
int pintar_replicas_unicas(int linhas, int colunas, PilhaAlteracoes *hist);
int ajuda_automatica(int linhas, int colunas, PilhaAlteracoes *hist);
int resolver_jogo(Celula **tabuleiro, int linhas, int colunas); 
void resolve_jogo(int linhas, int colunas, PilhaAlteracoes *hist);
// Helper functions
int check_vizinhos_validos(int i, int j, int linhas, int colunas);

#endif