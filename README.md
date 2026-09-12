# Problema da Mochila Binária — Heurísticas Gulosas

[![C](https://img.shields.io/badge/C-GCC-00599C?logo=c\&logoColor=white)](https://en.wikipedia.org/wiki/C_%28programming_language%29)
[![Algorithm](https://img.shields.io/badge/Algorithm-Heurística%20Gulosa-blueviolet)](#)
[![Sorting](https://img.shields.io/badge/Sorting-Merge%20Sort-2E8B57)](#)
[![USP](https://img.shields.io/badge/USP-EACH-red)](https://www5.each.usp.br/)

> Implementação em C do problema da mochila binária utilizando um algoritmo guloso e três critérios de seleção.

Projeto desenvolvido para a disciplina **ACH2002 — Introdução à Análise de Algoritmos**, da Universidade de São Paulo (USP — EACH) no qual o programa lê uma coleção de objetos, ordena essa coleção por diferentes critérios e simula o preenchimento de uma mochila com capacidade limitada.

## Sobre o projeto

No problema da mochila binária, cada objeto pode ser escolhido no máximo uma vez, assim o objeto é representado pela estrutura abaixo, que armazena seu peso, seu valor e o ponteiro para o próximo elemento da lista ligada:
```c
typedef struct Objeto {
    int peso;
    int valor;
    struct Objeto *prox;
} Objeto;
```

O objetivo é selecionar objetos sem ultrapassar a capacidade da mochila, buscando maximizar o valor carregado. Esta implementação utiliza uma heurística gulosa: os objetos são ordenados e considerados nessa ordem, sem explorar todas as combinações possíveis. Por isso, os resultados podem não corresponder à solução ótima do problema.

Ao comparar as estratégias, o programa executa o preenchimento três vezes:

| Critério | Ordem de seleção | Critério de desempate |
| -------- | ---------------- | --------------------- |
| Menor peso | Peso crescente | Maior valor |
| Maior valor | Valor decrescente | Menor peso |
| Maior valor dividido pelo peso | Razão valor/peso decrescente | Maior peso |

Em cada execução, a lista ligada de objetos é reordenada com **merge sort**. O programa não remove objetos da lista original, pois ele apenas atualiza sua ordem para aplicar o critério seguinte.

## Estrutura do Repositório

```text
problema-mochila-binaria/
│
├── src/
│   └── mochila.c
│
├── tests/
│   ├── entrada2.txt
│   └── saida2.txt
│
├── docs/
│   └── EP2_IAA_2SEM2025.pdf
│
├── .gitignore
└── README.md
```

### Diretórios

| Diretório | Descrição |
| --------- | --------- |
| `src/` | Código-fonte do programa em C |
| `tests/` | Arquivo de entrada e saída esperada do caso de teste |
| `docs/` | Instruções do projeto|

## Formato do Arquivo de Entrada

A primeira linha deve conter a capacidade máxima da mochila e a quantidade de objetos. Cada uma das linhas seguintes deve conter o peso e o valor de um objeto:

```text
<capacidade> <número_de_objetos>
<peso_1> <valor_1>
<peso_2> <valor_2>
...
<peso_n> <valor_n>
```

Exemplo de `tests/entrada2.txt`:

```text
10 7
2 3
6 11
4 11
2 5
1 2
4 8
9 11
```

Nesse exemplo, a mochila possui capacidade `10` e existem `7` objetos disponíveis.

## Instalação e Uso

### Requisitos

* GCC;
* sistema compatível com compilação de programas C;
* arquivo de entrada no formato descrito acima.

### Compilação

Na raiz do repositório, execute:

```bash
gcc -Wall -Wextra -std=c11 -o mochila src/mochila.c
```

### Execução

O programa recebe dois argumentos: o arquivo de entrada e o arquivo de saída.

```bash
./mochila tests/entrada2.txt saida.txt
```

No Windows, utilizando MinGW, o executável pode ser iniciado com:

```powershell
.\mochila.exe tests\entrada2.txt saida.txt
```

Durante a execução, o programa exibe no terminal os objetos adicionados em cada estratégia. Ao final, os resultados resumidos são gravados no arquivo informado como segundo argumento.

## Exemplo de saída

Para o arquivo de entrada fornecido, `saida.txt` contém resultados neste formato:

```text
Testando critério de seleção: 'Menor Peso'
Peso máximo: 10
Peso usado: 9
Valor carregado: 21
Número de objetos: 4

Testando critério de seleção: 'Maior Valor'
Peso máximo: 10
Peso usado: 10
Valor carregado: 22
Número de objetos: 2

Testando critério de seleção: 'Maior Valor Dividido pelo Peso'
Peso máximo: 10
Peso usado: 10
Valor carregado: 24
Número de objetos: 3
```

O arquivo `tests/saida2.txt` contém a saída de referência do caso de teste, com pequenas diferenças de acentuação e formatação decorrentes do ambiente em que foi gerado.

## Funcionamento do algoritmo

O programa executa as seguintes etapas:

1. Lê a capacidade e os objetos do arquivo de entrada.
2. Armazena os objetos em uma lista ligada.
3. Ordena a lista com merge sort conforme o critério atual.
4. Percorre a lista e adiciona objetos enquanto houver capacidade disponível.
5. Registra o peso utilizado, o valor carregado e a quantidade de objetos escolhidos.
6. Repete o processo para os três critérios e grava os resultados.

O merge sort possui complexidade de tempo $O(n \log n)$ para ordenar os objetos e o preenchimento da mochila percorre a lista em $O(n)$, assim, cada estratégia executada tem custo assintótico dominado pela ordenação. Por fim, o armazenamento dos objetos utiliza uma lista ligada, com custo espacial $O(n)$.

> **Observação:** a implementação interrompe o preenchimento quando o próximo objeto ordenado não cabe na capacidade restante. Ela não continua procurando objetos posteriores que eventualmente poderiam caber.

## Limitações

* A heurística gulosa não garante o maior valor possível para todos os casos.
* O programa espera exatamente dois argumentos na linha de comando.
* A entrada deve conter a quantidade de pares `peso valor` indicada na primeira linha.
* Os pesos devem ser positivos, pois o critério de razão calcula `valor / peso`.

## Licença

Projeto Acadêmico apresentado à **Universidade de São Paulo — Escola de Artes, Ciências e Humanidades (USP — EACH)**.

© 2025 Ygor Araujo

[![GitHub](https://img.shields.io/badge/GitHub-contygor-181717?logo=github\&logoColor=white)](https://github.com/contygor)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Ygor%20Araujo-0A66C2?logo=linkedin\&logoColor=white)](https://www.linkedin.com/in/contygor/)
