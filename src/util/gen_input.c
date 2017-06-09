#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <util/debug.h>

int main(int argc, char *argv[]) {
  // primeiro argumento define k
  // segundo argumento (opcional) define a semente para a geração de dados aleatórios

  if(argc < 2) {
    fprintf(stderr, "Usage: gen_input <k> [seed]\n");
    exit(EXIT_FAILURE);
  }

  int k = atoi(argv[1]);
  check(2 < k && k < 21, "invalid input size. values must be in the range [3-20].");
  size_t n = (size_t) 1 << k; // n = 2**k

  int seed = 0;
  if(argc > 2)
    seed = atoi(argv[2]);
  srand48(seed);

  fprintf(stderr, "[INFO] k: %d, n: %zd, seed: %d\n", k, n, seed);

  mkdir("data", 0700);
  FILE *int_out = fopen("data/input_int", "w");
  check(int_out, "failed to open file 'data/input_int' for writing.");

  fprintf(stderr, "[INFO] writing integer data\n");
  // dados em ordem crescente
  for(size_t i = 1; i <= n; i++)
    fprintf(int_out, "%ld ", i);
  fputc('\n', int_out);

  // dados em ordem decrescente
  for(size_t i = 1; i <= n; i++)
    fprintf(int_out, "%ld ", n - i);
  fputc('\n', int_out);

  // dados desordenados
  for(size_t i = 1; i <= n; i++)
    fprintf(int_out, "%ld ", lrand48() % n);
  fputc('\n', int_out);

  // dados semi-ordenados em ordem crescente
  for(size_t i = 1; i <= n; i++)
    fprintf(int_out, "%ld ", i%8 != 0 ?  i : (n-i));
  fputc('\n', int_out);

  fclose(int_out);
  FILE *float_out = fopen("data/input_float", "w");
  check(float_out, "failed to open file 'data/input_float' for writing.");

  fprintf(stderr, "[INFO] writing floating point data\n");
  // dados em ordem crescente
  for(size_t i = 1; i <= n; i++)
    fprintf(float_out, "%.10f ", (double)i/n);
  fputc('\n', float_out);

  // dados em ordem decresente
  for(size_t i = 1; i <= n; i++)
    fprintf(float_out, "%.10f ", (double)(n-i)/n);
  fputc('\n', float_out);

  // dados desordenados
  for(size_t i = 1; i <= n; i++)
    fprintf(float_out, "%.10f ", drand48());
  fputc('\n', float_out);

  // dados semi-ordenados em ordem crescente
  for(size_t i = 1; i <= n; i++)
    fprintf(float_out, "%.10f ", i%8 != 0 ? ((double)i/n) : ((double)(n-i)/n));
  fputc('\n', float_out);

  fclose(float_out);
  fprintf(stderr, "[INFO] done\n");

  return 0;

  error:
  return 1;
}
