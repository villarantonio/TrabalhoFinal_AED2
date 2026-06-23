#include "hash.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Sentinelas: slots nunca usados têm valor=VAZIO; slots deletados têm valor=REMOVIDO.
   Índices válidos de vértices são >= 0, portanto -1 e -2 são seguros como sentinelas. */
#define VAZIO    (-1)
#define REMOVIDO (-2)

/* djb2 com tolower: garante que a busca seja case-insensitive */
static unsigned int calcular_hash(const char* chave, int capacidade) {
    unsigned long h = 5381;
    unsigned char c;
    while ((c = (unsigned char)*chave++) != '\0')
        h = h * 33 + (unsigned long)tolower(c);
    return (unsigned int)(h % (unsigned long)capacidade);
}

/* Compara duas strings ignorando maiúsculas/minúsculas */
static int chaves_iguais(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return (*a == '\0') && (*b == '\0');
}

/* Aloca tabela hash com todos os slots vazios */
HashTable* criar_hash(int capacidade) {
    HashTable* t = (HashTable*)malloc(sizeof(HashTable));
    if (!t) { perror("malloc HashTable"); exit(1); }

    t->capacidade = capacidade;
    t->tamanho    = 0;
    t->entradas   = (HashEntry*)malloc(capacidade * sizeof(HashEntry));
    if (!t->entradas) { perror("malloc entradas"); exit(1); }

    for (int i = 0; i < capacidade; i++) {
        t->entradas[i].chave = NULL;
        t->entradas[i].valor = VAZIO;
    }
    return t;
}

/* Libera toda a memória da tabela */
void destruir_hash(HashTable* t) {
    if (!t) return;
    for (int i = 0; i < t->capacidade; i++) {
        if (t->entradas[i].chave != NULL)
            free(t->entradas[i].chave);
    }
    free(t->entradas);
    free(t);
}

/* Insere o par (chave, valor) na tabela. A chave é copiada internamente. */
void hash_inserir(HashTable* t, const char* chave, int valor) {
    unsigned int idx = calcular_hash(chave, t->capacidade);
    /* sonda linearmente até encontrar slot vazio ou removido */
    while (t->entradas[idx].valor != VAZIO && t->entradas[idx].valor != REMOVIDO)
        idx = (idx + 1) % (unsigned int)t->capacidade;

    t->entradas[idx].chave = (char*)malloc(strlen(chave) + 1);
    if (!t->entradas[idx].chave) { perror("malloc chave"); exit(1); }
    strcpy(t->entradas[idx].chave, chave);
    t->entradas[idx].valor = valor;
    t->tamanho++;
}

/* Busca a chave e retorna o valor associado, ou -1 se não encontrado.
   A comparação é case-insensitive. */
int hash_buscar(HashTable* t, const char* chave) {
    unsigned int idx   = calcular_hash(chave, t->capacidade);
    unsigned int start = idx;
    do {
        if (t->entradas[idx].valor == VAZIO)
            return -1; /* slot vazio: chave não existe na tabela */
        if (t->entradas[idx].valor != REMOVIDO &&
            chaves_iguais(t->entradas[idx].chave, chave))
            return t->entradas[idx].valor;
        idx = (idx + 1) % (unsigned int)t->capacidade;
    } while (idx != start);
    return -1; /* tabela cheia sem encontrar (não ocorre com load factor < 0.5) */
}
