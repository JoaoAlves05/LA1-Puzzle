# LA1-Puzzle

![banner](https://img.shields.io/badge/C-Expert-green?style=flat-square)

**Puzzle das Letras** é um jogo de lógica em C, com interface colorida e comandos interativos, ideal para desafiar o raciocínio e impressionar em candidaturas técnicas!

## Como compilar e jogar

```sh
make puzzle      # Compila o jogo principal
./puzzle         # Inicia o jogo

make tests       # Compila todos os testes (ficheiros em test/)
./build/test_nome # Corre cada teste individualmente (exemplo: ./build/test_comandos)
```

## Exemplo de arranque

```
========================================
  _____  _    _  _______  _______ __     ______ 
 |  __ \| |  | ||___   / |___   /|  |   |  ____|
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
Prima ENTER para começar o jogo...
```

## Interface colorida

- Casas riscadas: **vermelho**
- Casas brancas: **verde**
- Mensagens de erro: **vermelho**
- Mensagens de sucesso: **verde**
- Menus e prompts destacados

## Comandos disponíveis durante o jogo

- `b <coord>`: Pintar casa de branco (ex: b a1)
- `r <coord>`: Riscar casa (ex: r b2)
- `a`: Aplicar regras de ajuda uma vez
- `A`: Aplicar ajuda até não haver mais alterações
- `R`: Resolver o puzzle automaticamente
- `g <ficheiro>`: Gravar estado atual
- `l <ficheiro>`: Carregar jogo de ficheiro
- `d`: Desfazer última alteração
- `D`: Desfazer todas as alterações
- `v`: Verificar violações atuais
- `s`: Sair

## Dicas

- Pode inserir o tabuleiro linha a linha, com feedback imediato.
- O comando `D` desfaz todas as alterações feitas desde o início, mas não faz reset puro ao tabuleiro original (para isso, implemente um comando `reset`).
- O código é robusto, validando todos os inputs e exibindo mensagens claras.
- Os testes automáticos devem ser colocados em ficheiros `.c` dentro da pasta `test/` e são compilados para `build/`.

---

## Licença

MIT

---

Desenvolvido com 💚 por um expert em C!