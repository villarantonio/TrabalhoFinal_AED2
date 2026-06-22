#ifndef METRICAS_H
#define METRICAS_H

#include "grafo.h"

int    grau_vertice(Grafo* g, int v);
int    vertice_maior_grau(Grafo* g);
double distancia_media_amostrada(Grafo* g, int num_amostras);
int    grafo_conexo(Grafo* g);
void   estatisticas_gerais(Grafo* g);

#endif /* METRICAS_H */
