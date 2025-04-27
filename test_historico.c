#include <CUnit/Basic.h>
#include "historico.h"

void test_empilhar_desempilhar(void) {
    PilhaEstados h;
    int l, c;
    inicializarPilha(&h);
    // prepara um tabuleiro 2x2
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            tabuleiro[i][j].original = 'a';
            tabuleiro[i][j].atual    = 'a';
        }
    CU_ASSERT_TRUE(empilhar(&h, 2, 2));
    // modifica
    tabuleiro[0][0].atual = 'A';
    CU_ASSERT_TRUE(desempilhar(&h, &l, &c));
    CU_ASSERT_EQUAL(l, 2);
    CU_ASSERT_EQUAL(c, 2);
    CU_ASSERT_EQUAL(tabuleiro[0][0].atual, 'a');
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("HistoricoSuite", NULL, NULL);
    CU_add_test(suite, "empilhar & desempilhar", test_empilhar_desempilhar);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}