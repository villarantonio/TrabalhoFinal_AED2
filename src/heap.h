#ifndef HEAP_H
#define HEAP_H

/* Par (vértice, distância) armazenado no heap */
typedef struct HeapNode {
    int    vertice;
    double distancia;
} HeapNode;

/* Heap mínimo indexado por vértice */
typedef struct MinHeap {
    int       tamanho;
    int       capacidade;
    int*      posicao;  /* posicao[v] = índice de v no array nos[] */
    HeapNode* nos;
} MinHeap;

MinHeap* criar_heap(int capacidade);
void     destruir_heap(MinHeap* h);
void     inserir_heap(MinHeap* h, int vertice, double distancia);
HeapNode extrair_minimo(MinHeap* h);
void     diminuir_chave(MinHeap* h, int vertice, double distancia);
int      heap_vazio(MinHeap* h);

#endif /* HEAP_H */
