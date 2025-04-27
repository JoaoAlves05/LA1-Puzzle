#include <CUnit/Basic.h>
#include <ctype.h>
#include "tabuleiro.h"
#include "verificacoes.h"

// Inicializa o tabuleiro a partir de uma string em row-major
static void init_tabuleiro(const char *data, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            char c = data[i * colunas + j];
            tabuleiro[i][j].original = tolower(c);
            tabuleiro[i][j].atual    = c;
        }
    }
}

void test_duplicados(void) {
    // 2x2, duas 'A' brancas na primeira linha
    init_tabuleiro("AAaa", 2, 2);
    CU_ASSERT_TRUE(contarViolacoes(2, 2) > 0);
}

void test_conectividade(void) {
    // 2x2, isola a segunda A
    init_tabuleiro("A#aA", 2, 2);
    CU_ASSERT_TRUE(contarViolacoes(2, 2) > 0);
}

int main(void) {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("VerificacoesSuite", NULL, NULL);
    CU_add_test(suite, "duplicados",       test_duplicados);
    CU_add_test(suite, "conectividade",    test_conectividade);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
