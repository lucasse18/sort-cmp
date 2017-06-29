#include <limits.h>

#include <sort/radix.h>

// modified version of code extracted from
// http://rosettacode.org/wiki/Sorting_algorithms/Radix_sort#C

static void swap(long unsigned *a, long unsigned *b) {
  long unsigned tmp = *a;
  *a = *b;
  *b = tmp;
}

static void radix_sort_u(long unsigned *from, long unsigned *to, unsigned bit)
{
  if (!bit || to < from + 1) return;

  long unsigned *ll = from, *rr = to - 1;
  for (;;) {
    /* find left most with bit, and right most without bit, swap */
    while (ll < rr && !(*ll & bit)) ll++;
    while (ll < rr &&  (*rr & bit)) rr--;
    if (ll >= rr) break;
    swap(ll, rr);
  }

  if (!(bit & *ll) && ll < to) ll++;
  bit >>= 1;

  radix_sort_u(from, ll, bit);
  radix_sort_u(ll, to, bit);
}

int radix_sort(long *A, size_t size) {
  size_t i;
  long unsigned *x = (long unsigned*) A;

  for (i = 0; i < size; i++)
    x[i] ^= INT_MIN;

  radix_sort_u(x, x + size, INT_MIN);

  for (i = 0; i < size; i++)
    x[i] ^= INT_MIN;

  return 0;
}
