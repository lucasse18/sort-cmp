#include <stdlib.h>

#include <sort/merge.h>

void merge(long *A, size_t size, size_t q) {
  long *B = malloc(size * sizeof(*B));

  for(size_t i = 0, j = q, k = 0; k < size; k++) {
    if(i == q)
      B[k] = A[j++];
    else if(j == size)
      B[k] = A[i++];
    else if(A[i] > A[j])
      B[k] = A[j++];
    else
      B[k] = A[i++];
  }

  for(size_t i = 0; i < size; i++)
    A[i] = B[i];

  free(B);
}

void merge_sort(long *A, size_t size) {
  if(size <= 1)
    return;

  size_t q = size >> 1; // q = floor(size/2)
  merge_sort(A, q);
  merge_sort(A + q, size - q);
  merge(A, size, q);
}
