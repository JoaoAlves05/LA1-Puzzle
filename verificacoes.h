#ifndef VERIFICACOES_H
#define VERIFICACOES_H

// Verifica duplicados, réplicas, vizinhos e conectividade ortogonal
void verificarEstado(int linhas, int colunas);

// Retorna número de violações encontradas no tabuleiro
int contarViolacoes(int linhas, int colunas);

#endif // VERIFICACOES_H