#include <CUnit/Basic.h>
#include "verificacoes.h"

// Função auxiliar para contar violações sem imprimir
extern int contarViolacoes(int linhas, int colunas);

void init_tabuleiro(char *data, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++) {
            char c = data[i * colunas + j];
            tabuleiro[i][j].original = tolower(c);
            tabuleiro[i][j].atual    = c;
        }
}

void test_duplicados(void) {
    init_tabuleiro("AAaa", 2, 2);
    CU_ASSERT_EQUAL(contarViolacoes(2, 2), 1);
}

void test_conectividade(void) {
    init_tabuleiro("A#aA", 2, 2);
    // A #  \ isolado 'A' em (1,2)
    CU_ASSERT_EQUAL(contarViolacoes(2, 2), 1);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("VerificacoesSuite", NULL, NULL);
    CU_add_test(suite, "duplicados", test_duplicados);
    CU_add_test(suite, "conectividade", test_conectividade);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}