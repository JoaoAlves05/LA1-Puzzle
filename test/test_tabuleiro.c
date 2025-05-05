#include <CUnit/Basic.h>
#include "tabuleiro.h"
#include <ctype.h>
#include "historico.h"

void test_input_coordenada_valida(void) {
    int l, c;
    CU_ASSERT_TRUE(input_coordenada("a1", &l, &c, 5, 5));
    CU_ASSERT_EQUAL(l, 0);
    CU_ASSERT_EQUAL(c, 0);
}

void test_input_coordenada_invalida(void) {
    int l, c;
    CU_ASSERT_FALSE(input_coordenada("z9", &l, &c, 5, 5));
    CU_ASSERT_FALSE(input_coordenada("1a", &l, &c, 5, 5));
}

void test_pintar_e_riscar(void) {
    inicializarTabuleiro(2, 2);
    tabuleiro[0][0].original = 'x';
    tabuleiro[0][0].atual    = 'x';
    // pintar
    CU_ASSERT_TRUE(pintarDeBranco(0, 0));
    CU_ASSERT_TRUE(isupper(tabuleiro[0][0].atual));
    CU_ASSERT_FALSE(pintarDeBranco(0, 0));
    // riscar
    tabuleiro[1][1].original = 'y';
    tabuleiro[1][1].atual    = 'y';
    CU_ASSERT_TRUE(riscarCasa(1, 1));
    CU_ASSERT_EQUAL(tabuleiro[1][1].atual, '#');
    CU_ASSERT_FALSE(riscarCasa(1, 1));
    liberarTabuleiro(2);
}

void test_pintar_e_riscar_com_historico(void) {
    PilhaAlteracoes h;
    inicializarPilha(&h);
    inicializarTabuleiro(2, 2);
    tabuleiro[0][0].original = 'x';
    tabuleiro[0][0].atual    = 'x';
    // pintar
    char valor_ant = tabuleiro[0][0].atual;
    char valor_novo = toupper(tabuleiro[0][0].original);
    CU_ASSERT_TRUE(pintarDeBranco(0, 0));
    empilhar(&h, 0, 0, valor_ant, valor_novo);
    CU_ASSERT_TRUE(isupper(tabuleiro[0][0].atual));
    CU_ASSERT_FALSE(pintarDeBranco(0, 0));
    // desfazer
    CU_ASSERT_TRUE(desfazer(&h));
    CU_ASSERT_EQUAL(tabuleiro[0][0].atual, 'x');
    // riscar
    tabuleiro[1][1].original = 'y';
    tabuleiro[1][1].atual    = 'y';
    valor_ant = tabuleiro[1][1].atual;
    valor_novo = '#';
    CU_ASSERT_TRUE(riscarCasa(1, 1));
    empilhar(&h, 1, 1, valor_ant, valor_novo);
    CU_ASSERT_EQUAL(tabuleiro[1][1].atual, '#');
    CU_ASSERT_FALSE(riscarCasa(1, 1));
    // desfazer
    CU_ASSERT_TRUE(desfazer(&h));
    CU_ASSERT_EQUAL(tabuleiro[1][1].atual, 'y');
    liberarTabuleiro(2);
    liberarPilha(&h);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("TabuleiroSuite", NULL, NULL);
    CU_add_test(suite, "input valida", test_input_coordenada_valida);
    CU_add_test(suite, "input invalida", test_input_coordenada_invalida);
    CU_add_test(suite, "pintar e riscar", test_pintar_e_riscar);
    CU_add_test(suite, "pintar/riscar com historico", test_pintar_e_riscar_com_historico);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}