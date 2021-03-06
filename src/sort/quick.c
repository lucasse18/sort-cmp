#include <sort/quick.h>

size_t partition(long *A, size_t size) {
  //long pivot = A[size >> 1]; // pivot = A[floor(size/2)]

  // by choosing the pivot as the last member, we'll get worst case time complexity, O(n**2), for
  // already sorted inputs (ascending or descending order).
  long pivot = A[size - 1];

  size_t i = 0;
  for(size_t j = size - 1; ; i++, j--) {
    while(A[i] < pivot) i++;
    while(A[j] > pivot) j--;

    if(i >= j) break;

    long tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
  }

  return i;
}

// tests showed it to work for 'size' up to 2**17. with a 'size' of 2**18 and greater a stack
// overflow occurs and the program is killed with SIGSEGV
void quick_sort(long *A, size_t size) {
  if(size < 2)
    return;

  size_t q = partition(A, size);

  quick_sort(A, q);
  quick_sort(A + q, size - q);
}
