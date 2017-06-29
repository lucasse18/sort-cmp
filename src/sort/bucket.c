#include <stdlib.h> // malloc, free

#include <sort/bucket.h>
#include <util/vector.h> // dynamic array implementation
#include <sort/insertion.h>

/*
pseudocode:
function bucket_sort(array, n) is
    buckets ← new array of n empty lists
for i = 0 to (length(array)-1) do
  insert array[i] into buckets[n * array[i]]
for i = 0 to n - 1 do
  insertion_sort(buckets[i], length(buckets[i]));
return the concatenation of buckets[0], ...., buckets[n-1]
*/

int bucket_sort(double *A, size_t size) {
  vector_t **buckets = malloc(size * sizeof(vector_t *));

  // create 'size' buckets
  for(size_t i = 0; i < size; i++)
    buckets[i] = vector_create(sizeof(double));

  // insert each element on its bucket
  for(size_t i = 0; i < size; i++)
    vector_append(buckets[(size_t) ((size - 1) * A[i])], &A[i]);

  // sort each bucket with insertion sort
  for(size_t i = 0; i < size; i++)
    insertion_sort_double((double *) vector_underlying(buckets[i]), vector_size(buckets[i]));

  // concatenate the buckets in the input array
  size_t A_index = 0;
  for (size_t i = 0; i < size; i++) {
    size_t bucket_size = vector_size(buckets[i]);
    double *underlying = vector_underlying(buckets[i]);

    for (size_t j = 0; j < bucket_size; j++)
      A[A_index++] = underlying[j];

    vector_destroy(buckets[i]);
  }

  free(buckets);
  return 0;
}
