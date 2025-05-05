#ifndef VERIFICACOES_H
#define VERIFICACOES_H

#include "historico.h"

int contarConectividade(int linhas, int colunas);
int riscar_duplicados(int linhas, int colunas, PilhaAlteracoes *hist);
int pintar_vizinhos_riscados(int linhas, int colunas, PilhaAlteracoes *hist);
int pintar_replicas_unicas(int linhas, int colunas, PilhaAlteracoes *hist);
int ajuda_automatica(int linhas, int colunas, PilhaAlteracoes *hist);
int ajuda_repetida(int linhas, int colunas, PilhaAlteracoes *hist);
int resolver_jogo(int linhas, int colunas, PilhaAlteracoes *hist);
int resolver_jogo_backtrack(int linhas, int colunas, PilhaAlteracoes *hist);
int contarVizinhos(int linhas, int colunas);
int contarTodasAsViolacoes(int linhas, int colunas);
int contarDuplicados(int linhas, int colunas);

#endif
