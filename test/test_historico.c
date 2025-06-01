#include <CUnit/Basic.h>
#include "historico.h"

// Testa empilhar e desfazer alterações no histórico
void test_empilhar_desfazer(void) {
    PilhaAlteracoes h;
    inicializarPilha(&h);
    inicializarTabuleiro(2, 2);
    tabuleiro[0][0].original = 'a';
    tabuleiro[0][0].atual    = 'a';
    // Simula pintar de branco
    empilhar(&h, 0, 0, 'a', 'A');
    tabuleiro[0][0].atual = 'A';
    CU_ASSERT_TRUE(desfazer(&h));
    CU_ASSERT_EQUAL(tabuleiro[0][0].atual, 'a');
    liberarTabuleiro(2);
    liberarPilha(&h);
}

// Função principal dos testes deste módulo
int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("HistoricoSuite", NULL, NULL);
    CU_add_test(suite, "empilhar & desfazer", test_empilhar_desfazer);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}