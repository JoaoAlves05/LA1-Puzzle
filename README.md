# LA1-Puzzle

![banner](https://img.shields.io/badge/C-Expert-green?style=flat-square)

**Puzzle das Letras** é um jogo de lógica em C, com interface colorida e comandos interativos, ideal para desafiar o raciocínio e impressionar em candidaturas técnicas!

## Como compilar e jogar

```sh
make all      # Compila o jogo e os testes
./puzzle      # Inicia o jogo
make tests    # Compila os testes (binário: tests/testes)
./tests/testes # Corre os testes (se existirem)
```

### Exemplo de arranque

```
   ========================================
    _____  _    _  _______  _______ __     ______ 
   | __ \\| |  | ||___   / |___   /|  |   |  ____|
   | |__) | |  | |   /  /     /  / |  |   | |__   
   |  ___/| |  | |  /  /     /  /  |  |   |  __|  
   | |    | |__| | /  /___  /  /__ |  |___| |____ 
   |_|    \\____/ |_______||______||______|______|
   
                D A S   L E T R A S  
    =============================================
Bem-vindo ao Puzzle das Letras!
Desenvolvido por um verdadeiro expert em C!

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

### Interface colorida

- Casas riscadas: **vermelho**
- Casas brancas: **verde**
- Mensagens de erro: **vermelho**
- Mensagens de sucesso: **verde**
- Menus e prompts destacados

### Comandos disponíveis

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

### Dicas

- Pode inserir o tabuleiro linha a linha, com feedback imediato.
- O comando `D` desfaz todas as alterações feitas desde o início, mas não faz reset puro ao tabuleiro original (para isso, implemente um comando `reset`).
- O código é robusto, validando todos os inputs e exibindo mensagens claras.

---