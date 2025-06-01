#ifndef COMANDOS_H
#define COMANDOS_H
#include "historico.h"

// Ficheiro de cabeçalhos para os comandos do jogo (interface do utilizador).

// Mostra o menu de comandos disponíveis.
void mostrar_menu();

// Processa o comando para pintar uma casa de branco.
void processar_comando_branco(PilhaAlteracoes *historico, int *linhas, int *colunas);

// Processa o comando para riscar uma casa.
void processar_comando_riscar(PilhaAlteracoes *historico, int *linhas, int *colunas);

// Processa o comando para gravar o estado do jogo num ficheiro.
void processar_comando_gravar(int linhas, int colunas);

// Processa o comando para carregar o estado do jogo de um ficheiro.
void processar_comando_carregar(int *linhas, int *colunas);

// Processa o comando para desfazer a última alteração.
void processar_comando_desfazer(PilhaAlteracoes *historico);

// Processa o comando para verificar violações no tabuleiro.
void processar_comando_verificar(int linhas, int colunas);

// Processa o comando para aplicar as regras de ajuda uma vez.
void processar_comando_ajuda(PilhaAlteracoes *historico, int linhas, int colunas);

// Processa o comando para aplicar as regras de ajuda repetidamente.
void processar_comando_ajuda_repetida(PilhaAlteracoes *historico, int linhas, int colunas);

// Processa o comando para resolver o puzzle automaticamente (backtracking).
void processar_comando_resolver(PilhaAlteracoes *historico, int linhas, int colunas);

// Processa o comando para resolver o puzzle, incluindo ajudas lógicas e backtracking.
void processar_comando_resolver_jogo(PilhaAlteracoes *historico, int linhas, int colunas);

// Processa o comando para desfazer todas as alterações.
void processar_comando_desfazer_tudo(PilhaAlteracoes *historico);

#endif