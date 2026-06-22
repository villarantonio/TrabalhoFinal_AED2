#include "metricas.h"
#include "dijkstra.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define INF (DBL_MAX / 2.0)

/* Conta o número de vizinhos do vértice v */
int grau_vertice(Grafo* g, int v) {
    int grau = 0;
    AdjNode* cur = g->adj[v];
    while (cur) {
        grau++;
        cur = cur->proximo;
    }
    return grau;
}

/* Retorna o índice do vértice com maior grau */
int vertice_maior_grau(Grafo* g) {
    int idx_max = 0;
    int grau_max = grau_vertice(g, 0);
    for (int i = 1; i < g->num_vertices; i++) {
        int g_i = grau_vertice(g, i);
        if (g_i > grau_max) {
            grau_max = g_i;
            idx_max  = i;
        }
    }
    return idx_max;
}

/* Estima a distância média entre pares usando amostragem aleatória.
   Ignora pares sem caminho (distância INF). */
double distancia_media_amostrada(Grafo* g, int num_amostras) {
    srand((unsigned int)time(NULL));

    double soma    = 0.0;
    int    validos = 0;

    for (int k = 0; k < num_amostras; k++) {
        int origem  = rand() % g->num_vertices;
        int destino = rand() % g->num_vertices;
        if (origem == destino) continue;

        ResultadoDijkstra* r = dijkstra(g, origem);
        double d = obter_distancia(r, destino);
        destruir_resultado(r);

        if (d >= 0.0) {
            soma += d;
            validos++;
        }
    }

    if (validos == 0) return -1.0;
    return soma / validos;
}

/* Verifica conexidade via BFS a partir do vértice 0.
   Retorna 1 se todos os vértices são alcançáveis, 0 caso contrário. */
int grafo_conexo(Grafo* g) {
    int  n        = g->num_vertices;
    int* visitado = (int*)calloc(n, sizeof(int));
    int* fila     = (int*)malloc(n * sizeof(int));
    if (!visitado || !fila) { perror("malloc BFS"); exit(1); }

    int inicio = 0, fim = 0;
    fila[fim++]   = 0;
    visitado[0]   = 1;
    int contagem  = 1;

    while (inicio < fim) {
        int u = fila[inicio++];
        AdjNode* viz = g->adj[u];
        while (viz) {
            if (!visitado[viz->destino]) {
                visitado[viz->destino] = 1;
                fila[fim++] = viz->destino;
                contagem++;
            }
            viz = viz->proximo;
        }
    }

    free(visitado);
    free(fila);
    return contagem == n;
}

/* Imprime um resumo estatístico do grafo */
void estatisticas_gerais(Grafo* g) {
    int mv = vertice_maior_grau(g);

    printf("\n=== Estatisticas Gerais ===\n");
    printf("Vertices           : %d\n", g->num_vertices);
    printf("Arestas            : %d\n", g->num_arestas);

    /* grau médio = 2*|E| / |V| */
    double grau_medio = (2.0 * g->num_arestas) / g->num_vertices;
    printf("Grau medio         : %.2f\n", grau_medio);

    printf("Municipio com mais conexoes: %s (grau %d)\n",
           g->nomes[mv], grau_vertice(g, mv));

    printf("Grafo conexo       : %s\n", grafo_conexo(g) ? "Sim" : "Nao");

    /* menor e maior grau */
    int g_min = grau_vertice(g, 0), g_max = grau_vertice(g, 0);
    for (int i = 1; i < g->num_vertices; i++) {
        int gi = grau_vertice(g, i);
        if (gi < g_min) g_min = gi;
        if (gi > g_max) g_max = gi;
    }
    printf("Grau minimo        : %d\n", g_min);
    printf("Grau maximo        : %d\n", g_max);
}
