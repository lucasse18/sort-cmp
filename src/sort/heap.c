#include <sort/heap.h>

size_t max (long *A, size_t n, size_t i, size_t j, size_t k) {
  size_t max = i;
  if(j < n && A[j] > A[max])
    max = j;

  if(k < n && A[k] > A[max])
    max = k;

  return max;
}

void heapify_down(long *A, size_t size, size_t i) {
  while(1) {
    size_t j = max(A, size, i, (i << 1) + 1, (i << 1) + 2);

    if(j == i)
      break;

    long tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
    i = j;
  }
}

void heap_sort(long *a, size_t size) {
  for(size_t i = (size - 2) >> 1; i != (size_t)-1; i--)
    heapify_down(a, size, i);

  const size_t size1 = size - 1;
  for(size_t i = 0; i < size; i++) {
    long tmp = a[size1 - i];
    a[size1 - i] = a[0];
    a[0] = tmp;
    heapify_down(a, size1 - i, 0);
  }
}
