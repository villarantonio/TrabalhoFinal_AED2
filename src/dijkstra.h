#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "grafo.h"

/* Resultado de uma execução do algoritmo de Dijkstra */
typedef struct ResultadoDijkstra {
    double* distancias;    /* distância mínima da origem até cada vértice */
    int*    predecessores; /* predecessor no caminho mínimo (-1 se nenhum) */
    int     num_vertices;
} ResultadoDijkstra;

ResultadoDijkstra* dijkstra(Grafo* g, int origem);
void               destruir_resultado(ResultadoDijkstra* r);
void               imprimir_caminho(Grafo* g, ResultadoDijkstra* r, int destino);
double             obter_distancia(ResultadoDijkstra* r, int destino);

#endif /* DIJKSTRA_H */
