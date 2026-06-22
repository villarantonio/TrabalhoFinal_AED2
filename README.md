# TP INF0287 — Sistema de Rotas e Caminhos Mínimos

Disciplina: INF0287 – Algoritmos e Estruturas de Dados 2 (UFG)  
Tema: Linha A — Sistema de Rotas de Municípios de Goiás

## Compilação

```bash
make
```

Ou manualmente:

```bash
gcc -Wall -Wextra -g -std=c99 -o rotas_goias \
    src/grafo.c src/heap.c src/dijkstra.c src/metricas.c src/main.c
```

## Execução

```bash
./rotas_goias
```

O programa carrega automaticamente o dataset em `dataset/municipios_goias.txt`.

## Limpeza

```bash
make clean
```

## Verificação de memória (Linux)

```bash
make valgrind
```

## Estrutura do projeto

```
.
├── Makefile
├── README.md
├── dataset/
│   └── municipios_goias.txt   (80 municípios, 153 arestas)
├── src/
│   ├── grafo.h / grafo.c      (lista de adjacência)
│   ├── heap.h  / heap.c       (heap mínimo indexado)
│   ├── dijkstra.h / dijkstra.c (algoritmo de Dijkstra)
│   ├── metricas.h / metricas.c (análise da rede)
│   └── main.c                 (menu interativo)
└── relatorio/
    └── relatorio.md
```

## Uso dos nomes de municípios

Use underscores no lugar de espaços ao digitar nomes no menu.  
Exemplos: `Goiania`, `Caldas_Novas`, `Rio_Verde`, `Aparecida_de_Goiania`.
