#include "heap.h"

#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Funções auxiliares internas                                          */
/* ------------------------------------------------------------------ */

static void trocar(MinHeap* h, int i, int j) {
    /* atualiza o mapa de posições antes de trocar */
    h->posicao[h->nos[i].vertice] = j;
    h->posicao[h->nos[j].vertice] = i;

    HeapNode tmp = h->nos[i];
    h->nos[i]    = h->nos[j];
    h->nos[j]    = tmp;
}

/* Sobe o nó na posição idx enquanto for menor que o pai */
static void subir(MinHeap* h, int idx) {
    while (idx > 0) {
        int pai = (idx - 1) / 2;
        if (h->nos[pai].distancia > h->nos[idx].distancia) {
            trocar(h, pai, idx);
            idx = pai;
        } else {
            break;
        }
    }
}

/* Desce o nó na posição idx enquanto houver filho menor */
static void descer(MinHeap* h, int idx) {
    int menor = idx;
    int esq   = 2 * idx + 1;
    int dir   = 2 * idx + 2;

    if (esq < h->tamanho && h->nos[esq].distancia < h->nos[menor].distancia)
        menor = esq;
    if (dir < h->tamanho && h->nos[dir].distancia < h->nos[menor].distancia)
        menor = dir;

    if (menor != idx) {
        trocar(h, idx, menor);
        descer(h, menor);
    }
}

/* ------------------------------------------------------------------ */
/* API pública                                                          */
/* ------------------------------------------------------------------ */

/* Aloca heap para até capacidade vértices */
MinHeap* criar_heap(int capacidade) {
    MinHeap* h = (MinHeap*)malloc(sizeof(MinHeap));
    if (!h) { perror("malloc MinHeap"); exit(1); }

    h->tamanho    = 0;
    h->capacidade = capacidade;

    h->nos = (HeapNode*)malloc(capacidade * sizeof(HeapNode));
    if (!h->nos) { perror("malloc nos"); exit(1); }

    h->posicao = (int*)malloc(capacidade * sizeof(int));
    if (!h->posicao) { perror("malloc posicao"); exit(1); }

    /* -1 indica que o vértice ainda não está no heap */
    for (int i = 0; i < capacidade; i++)
        h->posicao[i] = -1;

    return h;
}

/* Libera a memória do heap */
void destruir_heap(MinHeap* h) {
    if (!h) return;
    free(h->nos);
    free(h->posicao);
    free(h);
}

/* Insere (vertice, distancia) no heap */
void inserir_heap(MinHeap* h, int vertice, double distancia) {
    if (h->tamanho == h->capacidade) {
        fprintf(stderr, "Erro: heap cheio\n");
        exit(1);
    }
    int idx = h->tamanho;
    h->nos[idx].vertice    = vertice;
    h->nos[idx].distancia  = distancia;
    h->posicao[vertice]    = idx;
    h->tamanho++;
    subir(h, idx);
}

/* Remove e retorna o nó com menor distância */
HeapNode extrair_minimo(MinHeap* h) {
    if (h->tamanho == 0) {
        fprintf(stderr, "Erro: heap vazio em extrair_minimo\n");
        exit(1);
    }
    HeapNode minimo = h->nos[0];
    h->posicao[minimo.vertice] = -1; /* removido do heap */

    h->tamanho--;
    if (h->tamanho > 0) {
        h->nos[0] = h->nos[h->tamanho];
        h->posicao[h->nos[0].vertice] = 0;
        descer(h, 0);
    }
    return minimo;
}

/* Atualiza a distância de um vértice já presente no heap */
void diminuir_chave(MinHeap* h, int vertice, double distancia) {
    int idx = h->posicao[vertice];
    if (idx == -1) return; /* vértice não está no heap */
    h->nos[idx].distancia = distancia;
    subir(h, idx);
}

/* Retorna 1 se o heap estiver vazio */
int heap_vazio(MinHeap* h) {
    return h->tamanho == 0;
}
