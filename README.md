# Sistema de Rotas — Municípios de Goiás

> Trabalho Prático Final — INF0287 Algoritmos e Estruturas de Dados 2  
> Universidade Federal de Goiás (UFG) · Profa. Telma Woerle · 1º Semestre 2026

Sistema interativo de consulta de rotas rodoviárias entre 80 municípios do estado de Goiás, implementado em **C (C99)** com grafo de lista de adjacência e algoritmo de **Dijkstra** acelerado por **heap mínimo indexado**.

---

## Funcionalidades

| Opção | Descrição |
|-------|-----------|
| 1 | Exibir todos os municípios e suas conexões |
| 2 | **Caminho mínimo entre dois municípios** (Dijkstra) |
| 3 | Listar vizinhos diretos de um município |
| 4 | Município com maior número de conexões |
| 5 | Estatísticas gerais da rede |
| 6 | Verificar se a rede é conexa (BFS) |
| 7 | Distância média estimada por amostragem aleatória |
| 8 | Sair |

---

## Demonstração

```
==================================================
  SISTEMA DE ROTAS - MUNICIPIOS DE GOIAS
==================================================
  1. Mostrar informacoes do grafo
  2. Buscar caminho minimo entre dois municipios
  ...
==================================================
Opcao: 2

Nome do municipio de origem  (use _ para espacos): Goiania
Nome do municipio de destino (use _ para espacos): Caldas_Novas

Caminho: Goiania -> Edeia -> Hidrolandia -> Caldas_Novas
Distancia total: 213.0 km
```

```
Opcao: 2

Nome do municipio de origem  (use _ para espacos): Goiania
Nome do municipio de destino (use _ para espacos): Jatai

Caminho: Goiania -> Goiatuba -> Rio_Verde -> Jatai
Distancia total: 380.0 km
```

```
Opcao: 5

=== Estatisticas Gerais ===
Vertices           : 80
Arestas            : 161
Grau medio         : 4.03
Municipio com mais conexoes: Goiania (grau 17)
Grafo conexo       : Sim
Grau minimo        : 1
Grau maximo        : 17
```

---

## Dataset

O arquivo [`dataset/municipios_goias.txt`](dataset/municipios_goias.txt) contém:

- **80 municípios** reais de Goiás — Goiânia, Anápolis, Caldas Novas, Rio Verde, Jataí, Itumbiara, Catalão, Luziânia, Formosa, Pirenópolis, Posse e outros.
- **161 arestas** representando distâncias rodoviárias aproximadas em km.
- Grafo **não-dirigido e conexo** (todo município é alcançável a partir de qualquer outro).

Formato do arquivo:

```
NUM_VERTICES NUM_ARESTAS
NomeVertice0
NomeVertice1
...
origem destino distancia_km
...
```

---

## Estruturas de Dados e Algoritmos

```
Grafo (lista de adjacência)          Heap Mínimo Indexado
─────────────────────────────        ────────────────────────────
adj[0] → [1,18km] → [2,54km] →…     nos[]      posicao[]
adj[1] → [0,18km] → [3,12km] →…     (v, dist)  índice de v no heap
...                                  Subir/Descer em O(log V)
```

| Estrutura | Arquivo | Complexidade principal |
|-----------|---------|----------------------|
| Grafo (lista de adjacência) | `src/grafo.c` | Inserção O(1), espaço O(V+E) |
| Heap mínimo indexado | `src/heap.c` | Extrair-min e Diminuir-chave em O(log V) |
| Dijkstra | `src/dijkstra.c` | **O((V+E) log V)** |
| BFS (conectividade) | `src/metricas.c` | O(V+E) |

---

## Como Compilar e Executar

### Pré-requisitos

- GCC com suporte a C99
- Make (Linux/macOS) ou MinGW (Windows)

### Compilar

```bash
make
```

Ou manualmente:

```bash
gcc -Wall -Wextra -g -std=c99 -o rotas_goias \
    src/grafo.c src/heap.c src/dijkstra.c src/metricas.c src/main.c
```

### Executar

```bash
./rotas_goias
```

O dataset é carregado automaticamente de `dataset/municipios_goias.txt`.

### Limpar arquivos compilados

```bash
make clean
```

### Verificar vazamentos de memória (requer valgrind — Linux)

```bash
make valgrind
```

---

## Estrutura do Repositório

```
.
├── Makefile
├── README.md
├── dataset/
│   └── municipios_goias.txt      ← 80 municípios, 161 arestas
├── src/
│   ├── grafo.h / grafo.c         ← representação do grafo
│   ├── heap.h  / heap.c          ← heap mínimo indexado
│   ├── dijkstra.h / dijkstra.c   ← caminhos mínimos
│   ├── metricas.h / metricas.c   ← análise da rede
│   └── main.c                    ← menu interativo
└── relatorio/
    └── relatorio.md              ← relatório técnico completo
```

---

## Observação sobre nomes de municípios

Ao digitar nomes no menu, use **underscores** no lugar de espaços:

| Nome real | Digitar como |
|-----------|-------------|
| Caldas Novas | `Caldas_Novas` |
| Rio Verde | `Rio_Verde` |
| Aparecida de Goiânia | `Aparecida_de_Goiania` |
| Bom Jesus de Goiás | `Bom_Jesus_de_Goias` |

Nomes compostos seguem o mesmo padrão — sem acentos, espaços substituídos por `_`.
