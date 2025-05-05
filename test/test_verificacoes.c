#include <CUnit/Basic.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "tabuleiro.h"
#include "verificacoes.h"
#include "historico.h"

void init_tabuleiro_test(const char *data, int linhas, int colunas) {
    if (strlen(data) != (size_t)(linhas * colunas)) {
        fprintf(stderr, "Invalid test data: expected %d chars, got %zu\n", 
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

void test_duplicados_linha(void) {
    // Test: Two 'A's in first row
    init_tabuleiro_test("AAbcde", 1, 6);
    CU_ASSERT_EQUAL(contarDuplicados(1, 6), 1);
    liberarTabuleiro(1);
}

void test_vizinhos_invalidos(void) {
    // Test: # with no white neighbors
    init_tabuleiro_test("A#.B.#", 2, 3);
    CU_ASSERT_EQUAL(contarVizinhos(2, 3), 1);
    liberarTabuleiro(2);
}

void test_conectividade_ilhas(void) {
    // Test: Disconnected white cells
    // Place B at center so it's not orthogonally adjacent to A
    init_tabuleiro_test("A...B....", 3, 3);  // 9 chars
    CU_ASSERT_EQUAL(contarConectividade(3, 3), 1);
    liberarTabuleiro(3);
}

void test_resolver_exemplo(void) {
    // Original project example (25 chars)
    char *tab_ini = "ecadcdcdecbdceedecdeebacc";
    init_tabuleiro_test(tab_ini, 5, 5);
    
    PilhaAlteracoes hist;
    inicializarPilha(&hist);
    
    CU_ASSERT_TRUE(resolver_jogo(5, 5, &hist));
    CU_ASSERT_EQUAL(contarTodasAsViolacoes(5, 5), 0);
    
    // Verifica que só há maiúsculas ou # no tabuleiro final
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            char c = tabuleiro[i][j].atual;
            CU_ASSERT_TRUE(isupper(c) || c == '#');
        }
    }
    
    liberarPilha(&hist);
    liberarTabuleiro(5);
}

void test_ajuda_automatica(void) {
    // Test help applies all rules
    init_tabuleiro_test("aabbc#", 2, 3);
    
    PilhaAlteracoes hist;
    inicializarPilha(&hist);
    
    ajuda_automatica(2, 3, &hist);
    CU_ASSERT_TRUE(tabuleiro[0][1].atual == '#');
    CU_ASSERT_TRUE(isupper(tabuleiro[0][0].atual));
    
    liberarPilha(&hist);
    liberarTabuleiro(2);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Verification Tests", NULL, NULL);

    CU_add_test(suite, "Duplicates in Row", test_duplicados_linha);
    CU_add_test(suite, "Invalid Neighbors", test_vizinhos_invalidos);
    CU_add_test(suite, "Connectivity Islands", test_conectividade_ilhas);
    CU_add_test(suite, "Solver Example", test_resolver_exemplo);
    CU_add_test(suite, "Auto Help Rules", test_ajuda_automatica);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
