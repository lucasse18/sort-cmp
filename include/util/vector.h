#ifndef SQL_DATABASE_VECTOR_H
#define SQL_DATABASE_VECTOR_H


#include <stddef.h>

typedef struct UTIL_VECTOR vector_t;

vector_t *vector_create(size_t memb_size);
vector_t *vector_destroy(vector_t *vector);
size_t vector_size(vector_t *vector);
int vector_append(vector_t *vector, const void *data);
void *vector_get(vector_t *vector, size_t index);
size_t vector_search(vector_t *vector, const void *data, int (*compare(const void *a, const void *b)));
void *vector_underlying(vector_t *vector);

// AVISO: procedimento pouco eficiente em que o conteúdo do vetor a partir de 'index' é deslocado de
// uma unidade.
int vector_delete(vector_t *vector, size_t index);

#endif//SQL_DATABASE_VECTOR_H
