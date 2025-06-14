#include <CUnit/Basic.h>
#include "comandos.h"
#include "verificacoes.h"
#include "tabuleiro.h"
#include "historico.h"

// Implementação auxiliar para inicializar o tabuleiro de teste
void init_tabuleiro_test(const char *str, int linhas, int colunas) {
    inicializarTabuleiro(linhas, colunas);
    int idx = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j].original = str[idx];
            tabuleiro[i][j].atual = str[idx];
            idx++;
        }
    }
}

// Testa o comando de pintar uma casa de branco
void test_comando_branco(void) {
    PilhaAlteracoes hist;
    inicializarPilha(&hist);
    int linhas = 2, colunas = 2;
    inicializarTabuleiro(linhas, colunas);
    tabuleiro[0][0].original = 'a';
    tabuleiro[0][0].atual = 'a';
    processar_comando_branco(&hist, &linhas, &colunas);
    CU_ASSERT_EQUAL(tabuleiro[0][0].atual, 'A');
    CU_ASSERT_EQUAL(hist.topo, 0);
    liberarPilha(&hist);
    liberarTabuleiro(linhas);
}

// Testa o comando de resolução automática
void test_comando_resolver(void) {
    // Puzzle simples e resolúvel
    char *tab_ini = "abab";
    inicializarTabuleiro(2, 2);
    for(int i=0; i<2; i++) {
        for(int j=0; j<2; j++) {
            char c = tab_ini[i*2 + j];
            tabuleiro[i][j].original = c;
            tabuleiro[i][j].atual = c;
        }
    }
    PilhaAlteracoes hist;
    inicializarPilha(&hist);
    processar_comando_resolver(&hist, 2, 2);
    CU_ASSERT_EQUAL(contarTodasAsViolacoes(2, 2), 0);
    liberarPilha(&hist);
    liberarTabuleiro(2);
}

// Testa o comando de ajuda repetida
void test_comando_ajuda_repetida(void) {
    // Puzzle que necessita de várias ajudas
    init_tabuleiro_test("aabbaa", 2, 3);
    PilhaAlteracoes hist;
    inicializarPilha(&hist);
    processar_comando_ajuda_repetida(&hist, 2, 3);
    CU_ASSERT_TRUE(hist.topo > 2); // Deve fazer várias alterações
    CU_ASSERT_EQUAL(contarDuplicados(2, 3), 0);
    liberarPilha(&hist);
    liberarTabuleiro(2);
}

// Testa o comando de desfazer alterações
void test_comando_desfazer(void) {
    inicializarTabuleiro(1, 1);
    PilhaAlteracoes hist;
    inicializarPilha(&hist);
    // Faz e desfaz uma alteração
    tabuleiro[0][0].original = 'a';
    int linhas = 1, colunas = 1;
    processar_comando_branco(&hist, &linhas, &colunas);
    processar_comando_desfazer(&hist);
    CU_ASSERT_EQUAL(tabuleiro[0][0].atual, 'a');
    liberarPilha(&hist);
    liberarTabuleiro(1);
}

// Função principal dos testes deste módulo
int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Comandos", 0, 0);

    CU_add_test(suite, "Pintar Branco", test_comando_branco);
    CU_add_test(suite, "Resolver", test_comando_resolver);
    CU_add_test(suite, "Ajuda Repetida", test_comando_ajuda_repetida);
    CU_add_test(suite, "Desfazer", test_comando_desfazer);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}