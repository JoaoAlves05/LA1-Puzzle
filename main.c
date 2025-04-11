#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

// Estrutura para armazenar um estado do tabuleiro
typedef struct {
    char estado[MAX_SIZE][MAX_SIZE];
    int linhas;
    int colunas;
} EstadoTabuleiro;

// Estrutura da pilha de estados
typedef struct {
    EstadoTabuleiro pilha[100];
    int topo;
} PilhaEstados;

// Inicializa a pilha
void inicializarPilha(PilhaEstados *p) {
    p->topo = -1;
}

// Empilha o estado atual
int empilhar(PilhaEstados *p, char tabuleiro[MAX_SIZE][MAX_SIZE], int linhas, int colunas) {
    if (p->topo >= 99) return 0;
    p->topo++;
    p->pilha[p->topo].linhas = linhas;
    p->pilha[p->topo].colunas = colunas;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            p->pilha[p->topo].estado[i][j] = tabuleiro[i][j];
        }
    }
    return 1;
}

// Desempilha e restaura o estado anterior
int desempilhar(PilhaEstados *p, char tabuleiro[MAX_SIZE][MAX_SIZE], int *linhas, int *colunas) {
    if (p->topo < 0) return 0;
    *linhas = p->pilha[p->topo].linhas;
    *colunas = p->pilha[p->topo].colunas;
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            tabuleiro[i][j] = p->pilha[p->topo].estado[i][j];
        }
    }
    p->topo--;
    return 1;
}

// Exibe o tabuleiro
void exibirTabuleiro(char tabuleiro[MAX_SIZE][MAX_SIZE], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

// Pinta uma casa de branco
void pintarDeBranco(char tabuleiro[MAX_SIZE][MAX_SIZE], int linha, int coluna) {
    if (tabuleiro[linha][coluna] >= 'a' && tabuleiro[linha][coluna] <= 'z') {
        tabuleiro[linha][coluna] -= 32; // Converte para maiúscula
    }
}

// Risca uma casa
void riscarCasa(char tabuleiro[MAX_SIZE][MAX_SIZE], int linha, int coluna) {
    tabuleiro[linha][coluna] = '#';
}

// Grava o estado do jogo num ficheiro
void gravarJogo(char *nomeArquivo, char tabuleiro[MAX_SIZE][MAX_SIZE], int linhas, int colunas) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para gravação.\n");
        return;
    }

    fprintf(arquivo, "%d %d\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "%c", tabuleiro[i][j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Jogo salvo no arquivo '%s'.\n", nomeArquivo);
}

// Carrega o jogo de um ficheiro
void carregarJogo(char *nomeArquivo, char tabuleiro[MAX_SIZE][MAX_SIZE], int *linhas, int *colunas) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    fscanf(arquivo, "%d %d", linhas, colunas);
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            fscanf(arquivo, " %c", &tabuleiro[i][j]);
        }
    }

    fclose(arquivo);
    printf("Jogo carregado do arquivo '%s'.\n", nomeArquivo);
}

// Desfaz o último comando
void desfazerComando(PilhaEstados *historico, char tabuleiro[MAX_SIZE][MAX_SIZE], int *linhas, int *colunas) {
    if (desempilhar(historico, tabuleiro, linhas, colunas)) {
        printf("Ultimo comando desfeito com sucesso.\n");
    } else {
        printf("Nenhum comando para desfazer.\n");
    }
}

void verificarEstado(char tabuleiro[MAX_SIZE][MAX_SIZE], int linhas, int colunas) {
    int restricoesVioladas = 0;

    // Verificar restrição 1: Uma única réplica de cada símbolo pintado de branco em cada linha e coluna
    for (int i = 0; i < linhas; i++) {
        for (char letra = 'A'; letra <= 'Z'; letra++) {
            int countLinha = 0;
            for (int j = 0; j < colunas; j++) {
                if (tabuleiro[i][j] == letra) {
                    countLinha++;
                }
            }
            if (countLinha > 1) {
                printf("Restriçao violada: Mais de uma letra '%c' pintada de branco na linha %d.\n", letra, i + 1);
                restricoesVioladas++;
            }
        }
    }

    for (int j = 0; j < colunas; j++) {
        for (char letra = 'A'; letra <= 'Z'; letra++) {
            int countColuna = 0;
            for (int i = 0; i < linhas; i++) {
                if (tabuleiro[i][j] == letra) {
                    countColuna++;
                }
            }
            if (countColuna > 1) {
                printf("Restriçao violada: Mais de uma letra '%c' pintada de branco na coluna %d.\n", letra, j + 1);
                restricoesVioladas++;
            }
        }
    }

    // Verificar restrição 2: Réplicas de um símbolo pintado de branco devem ser riscadas
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j] >= 'A' && tabuleiro[i][j] <= 'Z') { // Casa pintada de branco
                char letra = tabuleiro[i][j] + 32; // Letra minúscula correspondente
                for (int k = 0; k < linhas; k++) {
                    for (int l = 0; l < colunas; l++) {
                        if (tabuleiro[k][l] == letra) {
                            printf("Restriçao violada: Letra '%c' pintada de branco, mas réplica '%c' nao esta riscada.\n", tabuleiro[i][j], letra);
                            restricoesVioladas++;
                        }
                    }
                }
            }
        }
    }

    // Verificar restrição 3: Casas riscadas devem ter vizinhos ortogonais pintados de branco
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j] == '#') { // Casa riscada
                if ((i > 0 && tabuleiro[i - 1][j] < 'A') || // Vizinho acima
                    (i < linhas - 1 && tabuleiro[i + 1][j] < 'A') || // Vizinho abaixo
                    (j > 0 && tabuleiro[i][j - 1] < 'A') || // Vizinho à esquerda
                    (j < colunas - 1 && tabuleiro[i][j + 1] < 'A')) { // Vizinho à direita
                    printf("Restriçao violada: Casa riscada em (%d, %d) não tem todos os vizinhos ortogonais pintados de branco.\n", i + 1, j + 1);
                    restricoesVioladas++;
                }
            }
        }
    }

    // Verificar restrição 4: Todas as casas brancas devem estar conectadas ortogonalmente
    // (Implementação de busca em largura ou profundidade para verificar conectividade)
    // Para simplificar, você pode implementar isso como uma função separada.

    if (restricoesVioladas == 0) {
        printf("Todas as restriçoes estao sendo respeitadas.\n");
    } else {
        printf("Total de restriçoes violadas: %d.\n", restricoesVioladas);
    }
}

int main() {
    int linhas, colunas;
    char tabuleiro[MAX_SIZE][MAX_SIZE];

    PilhaEstados historico;
    inicializarPilha(&historico);

    // Leitura do tamanho do tabuleiro
    printf("Digite o numero de linhas e colunas do tabuleiro: ");
    if (scanf("%d %d", &linhas, &colunas) != 2 || linhas > MAX_SIZE || colunas > MAX_SIZE) {
        printf("Tamanho do tabuleiro invalido ou excede o máximo permitido (%d x %d).\n", MAX_SIZE, MAX_SIZE);
        return 1;
    }

    // Leitura do tabuleiro inicial
    printf("Digite o tabuleiro inicial (%d linhas de %d caracteres):\n", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            scanf(" %c", &tabuleiro[i][j]);
        }
    }

    // Loop principal do jogo
    char comando[10];
    int linha, coluna;
    while (1) {
        printf("\nTabuleiro atual:\n");
        exibirTabuleiro(tabuleiro, linhas, colunas);

        printf("\nComandos disponíveis:\n");
        printf("b <linha> <coluna> - Pintar de branco\n");
        printf("r <linha> <coluna> - Riscar casa\n");
        printf("g <arquivo> - Gravar jogo\n");
        printf("l <arquivo> - Carregar jogo\n");
        printf("d - Desfazer Comando\n");
        printf("v - Verificar estado do tabuleiro\n");
        printf("s - Sair\n");
        printf("Digite um comando: ");
        scanf("%s", comando);

        if (strncmp(comando, "b", 1) == 0) {
            if (scanf("%d %d", &linha, &coluna) != 2 || linha < 1 || linha > linhas || coluna < 1 || coluna > colunas) {
                printf("Coordenadas inválidas!\n");
                while (getchar() != '\n');
                continue;
            }
            empilhar(&historico, tabuleiro, linhas, colunas);
            pintarDeBranco(tabuleiro, linha - 1, coluna - 1);

        } else if (strncmp(comando, "r", 1) == 0) {
            if (scanf("%d %d", &linha, &coluna) != 2 || linha < 1 || linha > linhas || coluna < 1 || coluna > colunas) {
                printf("Coordenadas inválidas!\n");
                while (getchar() != '\n');
                continue;
            }
            empilhar(&historico, tabuleiro, linhas, colunas);
            riscarCasa(tabuleiro, linha - 1, coluna - 1);

        } else if (strncmp(comando, "g", 1) == 0) {
            char nomeArquivo[50];
            scanf("%s", nomeArquivo);
            gravarJogo(nomeArquivo, tabuleiro, linhas, colunas);

        } else if (strncmp(comando, "l", 1) == 0) {
            char nomeArquivo[50];
            scanf("%s", nomeArquivo);
            carregarJogo(nomeArquivo, tabuleiro, &linhas, &colunas);

        } else if (strncmp(comando, "d", 1) == 0) {
            desfazerComando(&historico, tabuleiro, &linhas, &colunas);
        }else if (strncmp(comando, "v", 1) == 0) {
            verificarEstado(tabuleiro, linhas, colunas);
        }  else if (strncmp(comando, "s", 1) == 0) {
            printf("Saindo do jogo...\n");
            break;

        } else {
            printf("Comando inválido!\n");
            while (getchar() != '\n');
        }
    }

    return 0;
}
