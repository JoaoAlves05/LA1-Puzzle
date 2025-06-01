#ifndef VERIFICACOES_H
#define VERIFICACOES_H

#include "historico.h"

// Conta o número de violações de conectividade (ilhas de casas brancas separadas). Retorna 0 se todas as casas brancas estão conectadas, 1 caso contrário.
int contarConectividade(int linhas, int colunas);


// Risca casas duplicadas na mesma linha ou coluna. Retorna o número de alterações feitas.
int riscar_duplicados(int linhas, int colunas, PilhaAlteracoes *hist);


// Pinta de branco casas vizinhas a casas riscadas, se aplicável.Retorna o número de alterações feitas
int pintar_vizinhos_riscados(int linhas, int colunas, PilhaAlteracoes *hist);


// Pinta de branco casas que são a única possibilidade para uma letra numa linha ou coluna. Retorna o número de alterações feitas.
int pintar_replicas_unicas(int linhas, int colunas, PilhaAlteracoes *hist);


// Risca casas que, se fossem pintadas de branco, isolariam casas brancas. Retorna o número de alterações feitas.
int riscar_casas_isoladas(int linhas, int colunas, PilhaAlteracoes *hist);

// Aplica todas as regras de ajuda uma vez ao tabuleiro. Retorna o número de alterações feitas.
// int ajuda_automatica(int linhas, int colunas, PilhaAlteracoes *hist);

//Aplica as regras de ajuda repetidamente até não haver mais alterações. Retorna o número total de alterações feitas.
int ajuda_repetida(int linhas, int colunas, PilhaAlteracoes *hist);

// Resolve o puzzle usando backtracking exaustivo. Retorna 1 se encontrou solução válida, 0 caso contrário
int resolver_jogo_backtrack(int linhas, int colunas, PilhaAlteracoes *hist);

// Conta o número de casas riscadas sem vizinhos brancos.
int contarVizinhos(int linhas, int colunas);

// Conta o número total de violações das regras do puzzle.
int contarTodasAsViolacoes(int linhas, int colunas);

// Conta o número de duplicados (letras repetidas) em linhas e colunas.
int contarDuplicados(int linhas, int colunas);

// Aplica todas as regras de ajuda ao tabuleiro numa única passagem. Retorna o número de alterações feitas.
int aplicar_todas_regras(int linhas, int colunas, PilhaAlteracoes *hist);

#endif
