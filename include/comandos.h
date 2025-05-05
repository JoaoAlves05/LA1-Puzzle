#ifndef COMANDOS_H
#define COMANDOS_H
#include "historico.h"

void mostrar_menu();
void processar_comando_branco(PilhaAlteracoes *historico, int *linhas, int *colunas);
void processar_comando_riscar(PilhaAlteracoes *historico, int *linhas, int *colunas);
void processar_comando_gravar(int linhas, int colunas);
void processar_comando_carregar(int *linhas, int *colunas);
void processar_comando_desfazer(PilhaAlteracoes *historico);
void processar_comando_verificar(int linhas, int colunas);
void processar_comando_ajuda(PilhaAlteracoes *historico, int linhas, int colunas);
void processar_comando_ajuda_repetida(PilhaAlteracoes *historico, int linhas, int colunas);
void processar_comando_resolver(PilhaAlteracoes *historico, int linhas, int colunas);
void processar_comando_resolver_jogo(int linhas, int colunas);
#endif