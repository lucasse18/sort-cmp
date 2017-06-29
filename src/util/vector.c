#include <stdlib.h>
#include <string.h> //memcpy
#include <util/vector.h>

#define VECTOR_STARTING_SIZE 8

struct UTIL_VECTOR {
  size_t max_memb; /**< Quantidade máxima de membros armazenáveis no vetor */
  size_t memb_size; /**< Tamanho de cada membro do vetor */
  size_t memb_count; /**< Quantidade de membros já armazenados no vetor */
  void *data;
};

#define VECTOR_AT(vector, i) (vector->data + vector->memb_size * (i))
// o macro abaixo só é válido em um escopo onde 'vector' é uma variável vector_t inicializada
#define vector_at(i) VECTOR_AT(vector, i)

#define VECTOR_ASSIGN(dst, src, size) ((memcpy(dst, src, size)))
// o macro abaixo só é válido em um escopo onde 'vector' é uma variável vector_t inicializada
#define vector_assign(dst, src) (VECTOR_ASSIGN(dst, src, vector->memb_size))

// o macro abaixo só é válido em um escopo onde 'vector' é uma variável vector_t inicializada
// index não pode ser menor que zero, visto que é do tipo size_t então resta verificar se ultrapassa
// o limite superior
#define is_valid(index) ((index) < vector->memb_count)

vector_t *vector_create(size_t memb_size) {
  vector_t *new;
  if((new = malloc(sizeof(struct UTIL_VECTOR))) != NULL) {
    if((new->data = malloc(VECTOR_STARTING_SIZE * memb_size)) != NULL) {
      new->max_memb = VECTOR_STARTING_SIZE;
      new->memb_size = memb_size;
      new->memb_count = 0;
    } else {
      free(new);
      new = NULL;
    }
  }

  return new;
}

vector_t *vector_destroy(vector_t *vector) {
  if(vector != NULL) {
    if(vector->data != NULL)
      free(vector->data);
    free(vector);
  }
  return NULL;
}

//não faz parte da interface, não verifica validade de 'vector' por razões de desempenho
int vector_handle_space(vector_t *vector) {
  // se espaço insuficiente no vetor, então dobrar seu tamanho
  if(vector->memb_count == vector->max_memb) {
    vector->max_memb = vector->max_memb << 1; // max_memb *= 2
    vector->data = realloc(vector->data, vector->max_memb);
  }
  // se ocupação menor que 25% e tamanho alocado maior que inicial, então reduzir tamanho alocado à
  // metade
  else if(vector->memb_count < vector->max_memb >> 2 && vector->max_memb > VECTOR_STARTING_SIZE) {
    vector->max_memb = vector->max_memb >> 1; // max_memb /= 2
    vector->data = realloc(vector->data, vector->max_memb);
  }
  return 1;
}

size_t vector_size(vector_t *vector) {
  if(vector != NULL)
    return vector->memb_count;
  return 0;
}

int vector_append(vector_t *vector, const void *data) {
  if(vector != NULL) {
    if(vector_handle_space(vector)) {
      vector_assign(vector_at(vector->memb_count), data);
      vector->memb_count++;
      return 0; // 0, se sucesso
    }
    return 1; // 1, se está vazio ou índice está fora dos limites
  }
  return -1; // -1, se é nulo
}

int vector_delete(vector_t *vector, size_t index) {
  if(is_valid(index)) {
    // copia conteúdo à frente do índice removido para o endereço do índice removido
    vector->memb_count--;
    memmove(vector_at(index), vector_at(index + 1), (vector->memb_count - index) * vector->memb_size);
    return 0;
  }
  return 1;
}

void *vector_get(vector_t *vector, size_t index) {
  if(vector != NULL) {
    if(vector->memb_count > 0 && is_valid(index)) {
      return vector_at(index);
    }
  }
  return NULL;
}

size_t vector_search(vector_t *vector, const void *data, int (*compare(const void *a, const void *b))) {
  if(vector != NULL) {
    for(size_t i = 0; i < vector->memb_count; i++) {
      if(!compare(vector_at(i), data))
        return i;
    }
  }
  return (size_t) -1;
}

void *vector_underlying(vector_t *vector) {
  if(vector != NULL)
    return vector->data;
  return NULL;
}
