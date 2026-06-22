#include "grafo.h"
#include "dijkstra.h"
#include "metricas.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAMINHO_DATASET "dataset/municipios_goias.txt"
#define NUM_AMOSTRAS    200

/* ------------------------------------------------------------------ */
/* Funções auxiliares de interface                                      */
/* ------------------------------------------------------------------ */

static void imprimir_menu(void) {
    printf("\n==================================================\n");
    printf("  SISTEMA DE ROTAS - MUNICIPIOS DE GOIAS\n");
    printf("==================================================\n");
    printf("  1. Mostrar informacoes do grafo\n");
    printf("  2. Buscar caminho minimo entre dois municipios\n");
    printf("  3. Listar vizinhos de um municipio\n");
    printf("  4. Municipio com maior numero de conexoes\n");
    printf("  5. Estatisticas gerais da rede\n");
    printf("  6. Verificar se a rede e conexa\n");
    printf("  7. Distancia media entre municipios (amostragem)\n");
    printf("  8. Sair\n");
    printf("==================================================\n");
    printf("Opcao: ");
}

/* Lê uma string do stdin descartando o '\n' final */
static void ler_string(char* buf, int tamanho) {
    if (fgets(buf, tamanho, stdin)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n')
            buf[len - 1] = '\0';
    }
}

/* ------------------------------------------------------------------ */
/* Handlers de cada opção do menu                                       */
/* ------------------------------------------------------------------ */

static void opcao_mostrar_grafo(Grafo* g) {
    imprimir_grafo(g);
}

static void opcao_caminho_minimo(Grafo* g) {
    char nome_orig[256], nome_dest[256];

    printf("Nome do municipio de origem  (use _ para espacos): ");
    ler_string(nome_orig, sizeof(nome_orig));

    printf("Nome do municipio de destino (use _ para espacos): ");
    ler_string(nome_dest, sizeof(nome_dest));

    int orig = buscar_vertice_por_nome(g, nome_orig);
    int dest = buscar_vertice_por_nome(g, nome_dest);

    if (orig == -1) {
        printf("Municipio de origem '%s' nao encontrado.\n", nome_orig);
        return;
    }
    if (dest == -1) {
        printf("Municipio de destino '%s' nao encontrado.\n", nome_dest);
        return;
    }

    ResultadoDijkstra* r = dijkstra(g, orig);
    imprimir_caminho(g, r, dest);
    destruir_resultado(r);
}

static void opcao_listar_vizinhos(Grafo* g) {
    char nome[256];
    printf("Nome do municipio (use _ para espacos): ");
    ler_string(nome, sizeof(nome));

    int idx = buscar_vertice_por_nome(g, nome);
    if (idx == -1) {
        printf("Municipio '%s' nao encontrado.\n", nome);
        return;
    }

    printf("\nVizinhos de %s (grau %d):\n", g->nomes[idx], grau_vertice(g, idx));
    AdjNode* cur = g->adj[idx];
    int i = 1;
    while (cur) {
        printf("  %d. %-35s %.1f km\n", i++, g->nomes[cur->destino], cur->peso);
        cur = cur->proximo;
    }
}

static void opcao_maior_grau(Grafo* g) {
    int mv = vertice_maior_grau(g);
    printf("\nMunicipio com mais conexoes: %s\n", g->nomes[mv]);
    printf("Numero de conexoes (grau)  : %d\n", grau_vertice(g, mv));
}

static void opcao_estatisticas(Grafo* g) {
    estatisticas_gerais(g);
}

static void opcao_conexo(Grafo* g) {
    if (grafo_conexo(g))
        printf("\nA rede e CONEXA: todos os municipios estao interligados.\n");
    else
        printf("\nA rede NAO e conexa: existem municipios isolados.\n");
}

static void opcao_distancia_media(Grafo* g) {
    printf("\nCalculando distancia media com %d pares aleatorios...\n", NUM_AMOSTRAS);
    double media = distancia_media_amostrada(g, NUM_AMOSTRAS);
    if (media < 0.0)
        printf("Nao foi possivel calcular (nenhum par com caminho valido).\n");
    else
        printf("Distancia media estimada: %.1f km\n", media);
}

/* ------------------------------------------------------------------ */
/* Ponto de entrada                                                     */
/* ------------------------------------------------------------------ */

int main(void) {
    printf("Carregando dataset...\n");
    Grafo* g = carregar_arquivo(CAMINHO_DATASET);
    printf("Grafo carregado: %d municipios, %d conexoes.\n",
           g->num_vertices, g->num_arestas);

    char entrada[16];
    int  opcao;
    int  rodando = 1;

    while (rodando) {
        imprimir_menu();
        ler_string(entrada, sizeof(entrada));

        if (sscanf(entrada, "%d", &opcao) != 1) {
            printf("Entrada invalida. Digite um numero de 1 a 8.\n");
            continue;
        }

        switch (opcao) {
            case 1: opcao_mostrar_grafo(g);    break;
            case 2: opcao_caminho_minimo(g);   break;
            case 3: opcao_listar_vizinhos(g);  break;
            case 4: opcao_maior_grau(g);       break;
            case 5: opcao_estatisticas(g);     break;
            case 6: opcao_conexo(g);           break;
            case 7: opcao_distancia_media(g);  break;
            case 8: rodando = 0;               break;
            default:
                printf("Opcao invalida. Escolha entre 1 e 8.\n");
        }
    }

    destruir_grafo(g);
    printf("\nPrograma encerrado. Ate logo!\n");
    return 0;
}
