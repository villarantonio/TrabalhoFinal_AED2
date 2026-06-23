#ifndef HASH_H
#define HASH_H

/* Entrada da tabela hash: chave (string) -> valor (índice do vértice) */
typedef struct {
    char* chave;
    int   valor;
} HashEntry;

/* Tabela hash com open-addressing e linear probing */
typedef struct {
    HashEntry* entradas;
    int        capacidade;
    int        tamanho;
} HashTable;

HashTable* criar_hash(int capacidade);
void       destruir_hash(HashTable* t);
void       hash_inserir(HashTable* t, const char* chave, int valor);
int        hash_buscar(HashTable* t, const char* chave);

#endif /* HASH_H */
