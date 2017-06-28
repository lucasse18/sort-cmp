#include <stdlib.h>

#include <sort/counting.h>

int counting_sort(long *A, size_t size) {
  long min = 1, max = size;
  long range = max - min + 1;
  long *count = calloc((size_t) range, sizeof(long));

  for(size_t i = 0; i < size; i++)
    count[A[i] - min]++;

  for(long i = min, z = 0; i <= max; i++) {
    for(size_t j = 0; j < count[i - min]; j++)
      A[z++] = i;
  }

  free(count);
  return 0;
}
