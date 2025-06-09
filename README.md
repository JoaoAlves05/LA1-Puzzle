# LA1-Puzzle

![banner](https://img.shields.io/badge/C-Expert-green?style=flat-square)

**Puzzle das Letras** é um jogo de lógica em C, com interface colorida e comandos interativos, ideal para desafiar o raciocínio e impressionar em candidaturas técnicas!

---

## Como compilar e jogar

```sh
make puzzle      # Compila o jogo principal
./puzzle         # Inicia o jogo

make tests       # Compila todos os testes (ficheiros em test/)
./build/test_nome # Corre cada teste individualmente (exemplo: ./build/test_comandos)
```

---

## Como funciona

Ao iniciar o programa, é apresentado um menu inicial:

- **1** - Novo jogo (inserir tabuleiro manualmente)
- **2** - Carregar jogo de ficheiro
- **0** - Sair

Se escolher "Novo jogo", pode inserir o tabuleiro linha a linha ou colar todas as linhas de uma vez.  
Se escolher "Carregar", o programa lê um ficheiro no formato esperado (com separadores `ORIGINAL` e `ATUAL`).  
Se houver erro ao carregar, volta ao menu inicial.

---

## Exemplo de arranque

```text
========================================
  _____  _    _  _______  _______ __     ______ 
 | __ \ | |  | ||___   / |___   /|  |   |  ____|
 | |__) | |  | |   /  /     /  / |  |   | |__   
 |  ___/| |  | |  /  /     /  /  |  |   |  __|  
 | |    | |__| | /  /___  /  /__ |  |___| |____ 
 |_|     \____/ |_______||______||______|______|

        D A S   L E T R A S                  
========================================
Bem-vindo ao Puzzle das Letras!
Desenvolvido por um verdadeiro expert em C!

======= MENU INICIAL =======
1 - Novo jogo (inserir tabuleiro manualmente)
2 - Carregar jogo de ficheiro
0 - Sair
============================
Escolha uma opção: 1
Quantas linhas e colunas terá o tabuleiro? (ex: 5 7)
Linhas (1-26): 4
Colunas (1-26): 5

Insira o tabuleiro inicial (4 linhas de 5 caracteres cada).
Use letras minúsculas para casas, '#' para casas riscadas.
Exemplo de linha: abcd#
Linha 1: abcd#
Linha 2: aefgh
Linha 3: ijklm
Linha 4: nopqr

Tabuleiro inicial inserido com sucesso!
```

---

## Interface colorida

- Casas riscadas: **vermelho**
- Casas brancas: **verde**
- Mensagens de erro: **vermelho**
- Mensagens de sucesso: **verde**
- Menus e prompts destacados

---

## Comandos disponíveis durante o jogo

- `b <coord>`: Pintar casa de branco (ex: b a1)
- `r <coord>`: Riscar casa (ex: r b2)
- `a`: Aplicar regras de ajuda uma vez
- `A`: Aplicar ajuda até não haver mais alterações
- `R`: Resolver o puzzle automaticamente (reset ao original + resolver)
- `g <ficheiro>`: Gravar estado atual (ficheiro no diretório atual)
- `l <ficheiro>`: Carregar jogo de ficheiro (ficheiro no diretório atual)
- `d`: Desfazer última alteração
- `D`: Desfazer todas as alterações feitas desde o início
- `v`: Verificar violações atuais
- `s`: Sair

---

## Dicas e funcionalidades

- Pode inserir o tabuleiro linha a linha, colar várias linhas de uma vez, ou corrigir linhas antes de confirmar.
- O comando `D` desfaz todas as alterações feitas desde o início, voltando ao estado original do tabuleiro.
- O comando `R` resolve o puzzle automaticamente, começando sempre do tabuleiro original.
- O código valida todos os inputs e exibe mensagens claras.
- Os testes automáticos devem ser colocados em ficheiros `.c` dentro da pasta `test/` e são compilados para `build/`.

---

## Estrutura dos ficheiros de jogo

Os ficheiros de jogo guardam:

- Dimensões do tabuleiro
- Secção `ORIGINAL` (tabuleiro inicial)
- Secção `ATUAL` (estado atual)

```text
4 5
ORIGINAL
a b c d #
a e f g h
i j k l m
n o p q r
ATUAL
a b c d #
a e f g h
i j k l m
n o p q r
```

---

## Licença

MIT

---

Desenvolvido com 💚 por um expert em C eheheh!
