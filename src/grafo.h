#ifndef GRAFO_H
#define GRAFO_H

#include "hash.h"

/* Nó da lista de adjacência */
typedef struct AdjNode {
    int destino;
    double peso;
    struct AdjNode* proximo;
} AdjNode;

/* Grafo representado por lista de adjacência */
typedef struct Grafo {
    int num_vertices;
    int num_arestas;
    char** nomes;      /* nome de cada vértice */
    AdjNode** adj;     /* vetor de cabeças de lista */
    HashTable* tabela; /* tabela hash: nome -> índice do vértice */
} Grafo;

Grafo* criar_grafo(int num_vertices);
void   destruir_grafo(Grafo* g);
void   adicionar_aresta(Grafo* g, int origem, int destino, double peso);
Grafo* carregar_arquivo(const char* caminho);
void   imprimir_grafo(Grafo* g);
int    buscar_vertice_por_nome(Grafo* g, const char* nome);

#endif /* GRAFO_H */
