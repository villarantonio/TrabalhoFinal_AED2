#include "grafo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Aloca e inicializa um grafo com num_vertices vértices */
Grafo* criar_grafo(int num_vertices) {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    if (!g) { perror("malloc Grafo"); exit(1); }

    g->num_vertices = num_vertices;
    g->num_arestas  = 0;

    g->nomes = (char**)calloc(num_vertices, sizeof(char*));
    if (!g->nomes) { perror("calloc nomes"); exit(1); }

    g->adj = (AdjNode**)calloc(num_vertices, sizeof(AdjNode*));
    if (!g->adj) { perror("calloc adj"); exit(1); }

    /* 163 é primo e dá fator de carga < 0,5 para até 80 entradas */
    g->tabela = criar_hash(163);

    return g;
}

/* Libera toda a memória do grafo */
void destruir_grafo(Grafo* g) {
    if (!g) return;

    for (int i = 0; i < g->num_vertices; i++) {
        /* libera lista de adjacência do vértice i */
        AdjNode* cur = g->adj[i];
        while (cur) {
            AdjNode* prox = cur->proximo;
            free(cur);
            cur = prox;
        }
        free(g->nomes[i]);
    }
    free(g->adj);
    free(g->nomes);
    destruir_hash(g->tabela);
    free(g);
}

/* Insere aresta não-dirigida (origem→destino e destino→origem) */
void adicionar_aresta(Grafo* g, int origem, int destino, double peso) {
    /* aresta origem → destino */
    AdjNode* no1 = (AdjNode*)malloc(sizeof(AdjNode));
    if (!no1) { perror("malloc AdjNode"); exit(1); }
    no1->destino  = destino;
    no1->peso     = peso;
    no1->proximo  = g->adj[origem];
    g->adj[origem] = no1;

    /* aresta destino → origem */
    AdjNode* no2 = (AdjNode*)malloc(sizeof(AdjNode));
    if (!no2) { perror("malloc AdjNode"); exit(1); }
    no2->destino  = origem;
    no2->peso     = peso;
    no2->proximo  = g->adj[destino];
    g->adj[destino] = no2;

    g->num_arestas++;
}

/* Carrega grafo a partir do arquivo de dataset */
Grafo* carregar_arquivo(const char* caminho) {
    FILE* fp = fopen(caminho, "r");
    if (!fp) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s'\n", caminho);
        exit(1);
    }

    int num_vertices, num_arestas;
    if (fscanf(fp, "%d %d", &num_vertices, &num_arestas) != 2) {
        fprintf(stderr, "Erro: formato invalido na primeira linha de '%s'\n", caminho);
        fclose(fp);
        exit(1);
    }

    Grafo* g = criar_grafo(num_vertices);

    /* lê os nomes dos vértices */
    for (int i = 0; i < num_vertices; i++) {
        char buf[256];
        if (fscanf(fp, "%255s", buf) != 1) {
            fprintf(stderr, "Erro: esperado nome do vertice %d\n", i);
            destruir_grafo(g);
            fclose(fp);
            exit(1);
        }
        g->nomes[i] = (char*)malloc(strlen(buf) + 1);
        if (!g->nomes[i]) { perror("malloc nome"); exit(1); }
        strcpy(g->nomes[i], buf);
        hash_inserir(g->tabela, g->nomes[i], i);
    }

    /* lê as arestas */
    for (int i = 0; i < num_arestas; i++) {
        int    orig, dest;
        double dist;
        if (fscanf(fp, "%d %d %lf", &orig, &dest, &dist) != 3) {
            fprintf(stderr, "Erro: formato invalido na aresta %d\n", i + 1);
            destruir_grafo(g);
            fclose(fp);
            exit(1);
        }
        if (orig < 0 || orig >= num_vertices || dest < 0 || dest >= num_vertices) {
            fprintf(stderr, "Erro: indices fora do intervalo na aresta %d\n", i + 1);
            destruir_grafo(g);
            fclose(fp);
            exit(1);
        }
        adicionar_aresta(g, orig, dest, dist);
    }

    fclose(fp);
    return g;
}

/* Imprime lista de adjacência do grafo */
void imprimir_grafo(Grafo* g) {
    printf("\n=== Grafo: %d vertices, %d arestas ===\n",
           g->num_vertices, g->num_arestas);
    for (int i = 0; i < g->num_vertices; i++) {
        printf("[%2d] %-35s ->", i, g->nomes[i]);
        AdjNode* cur = g->adj[i];
        while (cur) {
            printf(" %s(%.0fkm)", g->nomes[cur->destino], cur->peso);
            cur = cur->proximo;
        }
        printf("\n");
    }
}

/* Retorna o índice do vértice com esse nome (case-insensitive), ou -1 se não encontrado */
int buscar_vertice_por_nome(Grafo* g, const char* nome) {
    return hash_buscar(g->tabela, nome);
}
