# Ligue 4++ (Connect 4++)



## Sobre o Projeto

O **Ligue 4++** é uma versão expandida e aprimorada do clássico jogo de tabuleiro "Ligue 4" (Connect 4), desenvolvida em linguagem **C**. Este projeto foi idealizado e construído como parte das atividades acadêmicas na Universidade Federal de Sergipe (UFS), unindo os conceitos fundamentais de programação estruturada com lógicas avançadas de jogos.

Além das regras tradicionais onde o objetivo é alinhar 4 peças da mesma cor, esta versão introduz mecânicas inovadoras que trazem mais dinamismo e estratégia para as partidas, operando diretamente no terminal com uma interface colorida e amigável.

## Funcionalidades e Mecânicas Especiais

O jogo conta com um grid padrão de 6x7, simulação de gravidade e as seguintes implementações de destaque:

*   **Fichas Especiais:**
    * **Fichas Explosivas:** Utilizam lógica de **recursão** para calcular reações em cadeia, destruindo peças ao redor e alterando drasticamente o tabuleiro.
    * **Fichas Portais:** Permitem estratégias únicas de reposicionamento das jogadas.
*   **Modos de Jogo:**
    *   👤 vs 👤 (Jogador vs Jogador)
    *   👤 vs 🤖 (Jogador vs Máquina)
    *   🤖 vs 🤖 (Máquina vs Máquina - Modo Automático)
*   **Hall dos Campeões:** Sistema de persistência de dados utilizando arquivos `.txt` (leitura e escrita) para armazenar `structs` com o histórico e ranking dos melhores jogadores.
*   **Interface no Terminal:** Utilização de códigos de escape ANSI para colorir as peças (amarelo, ciano, vermelho, magenta), proporcionando uma experiência visual muito mais polida.

## Tecnologias Utilizadas

*   **Linguagem:** C
*   **Ambiente de Desenvolvimento (IDE):** Dev-C++
*   **Conceitos Aplicados:** Estruturas de dados (matrizes, structs), Recursividade, Manipulação de Arquivos (`fopen`, `fprintf`, `fscanf`), Lógica de Validação e Interface via Console.

## 🚀 Como Executar

### Pré-requisitos
Para rodar este jogo, você precisará de um compilador de C (como o GCC) ou uma IDE como o Dev-C++ instalada na sua máquina.

### Passos
1. Faça o clone deste repositório:
   ```bash
   git clone [https://github.com/](https://github.com/)[Lucas-Thiery]/[Ligue-4].git
