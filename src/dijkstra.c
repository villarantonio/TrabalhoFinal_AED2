#include "dijkstra.h"
#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define INF (DBL_MAX / 2.0)

/* Executa Dijkstra a partir do vértice 'origem'.
   Complexidade: O((V + E) log V) com heap mínimo indexado. */
ResultadoDijkstra* dijkstra(Grafo* g, int origem) {
    int n = g->num_vertices;

    ResultadoDijkstra* r = (ResultadoDijkstra*)malloc(sizeof(ResultadoDijkstra));
    if (!r) { perror("malloc ResultadoDijkstra"); exit(1); }

    r->num_vertices  = n;
    r->distancias    = (double*)malloc(n * sizeof(double));
    r->predecessores = (int*)malloc(n * sizeof(int));
    if (!r->distancias || !r->predecessores) { perror("malloc arrays dijkstra"); exit(1); }

    for (int i = 0; i < n; i++) {
        r->distancias[i]    = INF;
        r->predecessores[i] = -1;
    }
    r->distancias[origem] = 0.0;

    MinHeap* heap = criar_heap(n);
    inserir_heap(heap, origem, 0.0);

    /* insere todos os demais vértices com distância INF */
    for (int i = 0; i < n; i++) {
        if (i != origem)
            inserir_heap(heap, i, INF);
    }

    while (!heap_vazio(heap)) {
        HeapNode u = extrair_minimo(heap);

        /* distância já definitiva: relaxa vizinhos */
        AdjNode* viz = g->adj[u.vertice];
        while (viz) {
            int    v    = viz->destino;
            double nova = r->distancias[u.vertice] + viz->peso;

            if (nova < r->distancias[v]) {
                r->distancias[v]    = nova;
                r->predecessores[v] = u.vertice;
                diminuir_chave(heap, v, nova);
            }
            viz = viz->proximo;
        }
    }

    destruir_heap(heap);
    return r;
}

/* Libera o resultado */
void destruir_resultado(ResultadoDijkstra* r) {
    if (!r) return;
    free(r->distancias);
    free(r->predecessores);
    free(r);
}

/* Reconstrói e imprime o caminho da origem até 'destino' */
static void reconstruir(Grafo* g, ResultadoDijkstra* r, int v) {
    if (r->predecessores[v] == -1) {
        printf("%s", g->nomes[v]);
        return;
    }
    reconstruir(g, r, r->predecessores[v]);
    printf(" -> %s", g->nomes[v]);
}

void imprimir_caminho(Grafo* g, ResultadoDijkstra* r, int destino) {
    if (r->distancias[destino] >= INF) {
        printf("Nao existe caminho ate %s.\n", g->nomes[destino]);
        return;
    }
    printf("Caminho: ");
    reconstruir(g, r, destino);
    printf("\nDistancia total: %.1f km\n", r->distancias[destino]);
}

/* Retorna a distância mínima até 'destino', ou -1 se não há caminho */
double obter_distancia(ResultadoDijkstra* r, int destino) {
    if (r->distancias[destino] >= INF)
        return -1.0;
    return r->distancias[destino];
}
