#include <CUnit/Basic.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "tabuleiro.h"
#include "verificacoes.h"
#include "historico.h"

// Inicializa o tabuleiro para testes, preenchendo com os dados fornecidos
void init_tabuleiro_test(char *data, int linhas, int colunas) {
    if (strlen(data) != (size_t)(linhas * colunas)) {
        fprintf(stderr, "Dados de teste inválidos: esperados %d caracteres, recebidos %zu\n", 
                linhas*colunas, strlen(data));
        exit(EXIT_FAILURE);
    }
    inicializarTabuleiro(linhas, colunas);
    for(int i=0; i<linhas; i++) {
        for(int j=0; j<colunas; j++) {
            char c = data[i*colunas + j];
            tabuleiro[i][j].original = tolower(c);
            tabuleiro[i][j].atual = c;
        }
    }
}

// Testa a deteção de duplicados numa linha
void test_duplicados_linha(void) {
    init_tabuleiro_test("AAbcde", 1, 6);
    CU_ASSERT_EQUAL(contarDuplicados(1, 6), 1);
    liberarTabuleiro(1);
}

// Testa a deteção de casas riscadas sem vizinhos brancos
void test_vizinhos_invalidos(void) {
    init_tabuleiro_test("A#.B.#", 2, 3);
    CU_ASSERT_EQUAL(contarVizinhos(2, 3), 1);
    liberarTabuleiro(2);
}

// Testa a deteção de ilhas de casas brancas (não conectadas)
void test_conectividade_ilhas(void) {
    init_tabuleiro_test("A...B....", 3, 3);
    CU_ASSERT_EQUAL(contarConectividade(3, 3), 1);
    liberarTabuleiro(3);
}

// Testa a resolução automática de um exemplo do projeto
void test_resolver_exemplo(void) {
    char *tab_ini = "ecadcdcdecbdceedecdeebacc";
    init_tabuleiro_test(tab_ini, 5, 5);
    PilhaAlteracoes hist;
    inicializarPilha(&hist);
    CU_ASSERT_TRUE(resolver_jogo_backtrack(5, 5, &hist));
    CU_ASSERT_EQUAL(contarTodasAsViolacoes(5, 5), 0);
    liberarPilha(&hist);
    liberarTabuleiro(5);
}

// Testa se a ajuda automática aplica todas as regras corretamente
void test_ajuda_automatica(void) {
    init_tabuleiro_test("aabbc#", 2, 3);
    PilhaAlteracoes hist;
    inicializarPilha(&hist);
    aplicar_todas_regras(2, 3, &hist);
    CU_ASSERT_TRUE(tabuleiro[0][1].atual == '#');
    CU_ASSERT_TRUE(isupper(tabuleiro[0][0].atual));
    liberarPilha(&hist);
    liberarTabuleiro(2);
}

// Função principal dos testes deste módulo
int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Verification Tests", NULL, NULL);

    CU_add_test(suite, "Duplicados na Linha", test_duplicados_linha);
    CU_add_test(suite, "Vizinhos Inválidos", test_vizinhos_invalidos);
    CU_add_test(suite, "Ilhas de Conectividade", test_conectividade_ilhas);
    CU_add_test(suite, "Exemplo de Resolução", test_resolver_exemplo);
    CU_add_test(suite, "Ajuda Automática", test_ajuda_automatica);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
