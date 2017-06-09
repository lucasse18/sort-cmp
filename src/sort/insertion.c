#include <sort/insertion.h>

int insertion_sort(long *A, size_t size) {
  for(size_t i = 1; i < size; i++) {
    long key = A[i];
    size_t j = i - 1;
    while(j != (size_t)-1 && A[j] > key) {
      A[j + 1] = A[j];
      j--;
    }
    A[j+1] = key;
  }
  return 0;
}
