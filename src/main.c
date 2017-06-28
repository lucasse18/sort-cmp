#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <time.h>
#include <string.h>

#include <util/debug.h>
#include <sort/insertion.h>
#include <sort/merge.h>
#include <sort/heap.h>
#include <sort/quick.h>
#include <sort/counting.h>
#include <sort/radix.h>
#include <sort/bucket.h>

// prints a help message and exits.
void print_usage();
// read one line from the input file
void read_input_line(void *A, size_t size, int (*read_number)(void *n));
// helper functions for reading integers and floating point numbers from the input
int read_integer(void *n);
int read_float(void *n);
// helper functions for comparing integers and floating point numbers
int compare_integer(void *a, void *b);
int compare_float(void *a, void *b);

/*
void print_vec(double *A, size_t size) {
  for(size_t i = 0; i < size; i++)
    printf("%.4f ", A[i]);
  printf("\n");
}
*/

// if NDEBUG hasn't been defined, check if the output is in ascending order after sorting
#ifndef NDEBUG
#define assert_asc_order(A, size, cmp) ASSERT_ASC_ORDER((A), (size), (cmp))
void ASSERT_ASC_ORDER(void *A, size_t size, int (*compare)(void *, void *)) {
  for (size_t i = 1; i < size; i++)
    // this line assumes sizeof(long) == sizeof(double)
    check(compare(A + sizeof(long)*i, A + sizeof(long)*(i-1)) >= 0, "output not in ascending order!");

  return;
  error:
  exit(EXIT_FAILURE);
}
// if NDEBUG has been defined the function is left undefined and the macro expands to empty text
#else
#define assert_asc_order(A, size)
#endif

// global pointer to the input file
FILE *infile;

int main(int argc, char *argv[]) {
  infile = NULL;

  static int alg = 4;
  static size_t k = 3;
  static size_t samples = 1;

  static struct option opts[] = {
    {"help",       no_argument,       0, 'h'},
    {"algorithm",  required_argument, 0, 'a'},//default quick sort (4)
    {"size",       required_argument, 0, 'k'},//default 3 -> 2**3 = 8
    {"samples",    required_argument, 0, 's'},//default 1
    {0,           0,                  0,   0}
  };

  // the following loop parses the command line using 'getopt_long', which is a GNU extension
  int c;
  int index = 0;
  while((c = getopt_long(argc, argv, "ha:k:s:", opts, &index)) != -1) {
    switch(c) {
      case 0:
        break;

      case 'h':
        print_usage();
        exit(EXIT_SUCCESS);

      case 'a':
        alg = atoi(optarg);
        check(alg > 0 && alg < 8, "invalid algorithm code. values must be in the range [1-7].");
        break;

      case 'k':
        k = (size_t) atol(optarg);
        check(k > 2 && k < 21, "invalid input size. values must be in the range [3-20].");
        break;

      case 's':
        samples = (size_t) atoi(optarg);
        check(samples > 0, "invalid amount of samples. must be greater than zero.");
        break;

      case '?':
        // user passed an unsupported option
        // getopt_long already printed an error message
        exit(1);

      default:
        abort();
    }
  }

  check(argv[optind], "sort: no input file specified, stopping.");
  infile = fopen(argv[optind], "r");
  check(infile != NULL, "could not open file %s for reading.", argv[optind]);

  size_t n = (size_t) 1 << k; // n = 2**k

  int data_is_float = alg != 7 ? 0 : 1; // if alg_code == 7, data type to be used is floating point

  void *A, *B;
  int (*compare_fn)(void *, void *);
  int (*read_fn)(void *);
  if(data_is_float) {
    check_mem(A = malloc(n * sizeof(double)));
    check_mem(B = malloc(n * sizeof(double)));
    compare_fn = compare_float;
    read_fn = read_float;
  } else {
    check_mem(A = malloc(n * sizeof(long)));
    check_mem(B = malloc(n * sizeof(long)));
    compare_fn = compare_integer;
    read_fn = read_integer;
  }

  FILE *out;
  char out_filename[20], buff[20];
  char *sufixes[4] = {"asc.csv", "desc.csv", "rand.csv", "part.csv"};
  snprintf(out_filename, 20, "data/out_%d_", alg);
  strncpy(buff, out_filename, 20);

  // run for each of the four input lines (ascending, descending, random, partially sorted)
  for(size_t i = 0; i < 4; i++) {
    strncat(out_filename, sufixes[i], 20);
    check(out = fopen(out_filename, "a"), "could not open output file for writing");
    strncpy(out_filename, buff, 20);

    read_input_line(A, n, read_fn);
    // assumes sizeof(long) == sizeof(double)
    // copy the read vector into a temporary buffer so we can copy it back after sorting
    memcpy(B, A, n * sizeof(long));

    unsigned long long cpu_time = 0;

    // run algorithm 'samples' times
    for(size_t j = 0; j < samples; j++) {
      struct timespec start, stop;
      check(clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start) == 0, "function 'clock_gettime' returned error.");
      switch (alg) {
        case 1:
          insertion_sort(A, n);
          break;
        case 2:
          merge_sort(A, n);
          break;
        case 3:
          heap_sort(A, n);
          break;
        case 4:
          quick_sort(A, n);
          break;
        case 5:
          counting_sort(A, n);
          break;
        case 6:
          radix_sort(A, n);
          break;
        case 7:
          bucket_sort(A, n);
          break;
        default:
          abort();
      }
      check(clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop) == 0, "function 'clock_gettime' returned error.");
      // FIXME using a slow enough algorithm and/or a big enough input array may overflow 'cpu_time'
      cpu_time = (1000000000UL * (stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec);

      // check that the output has been sorted correctly
      assert_asc_order(A, n, compare_fn);

      // input_size,sorting_time(ns)
      fprintf(out, "%zd,%lld,%zd,%zd\n", n, cpu_time, i, j);

      // assumes sizeof(long) == sizeof(double)
      // copy the vector back from the temporary buffer
      memcpy(A, B, n * sizeof(long));
    }
    fclose(out);
  }

  free(A);
  free(B);
  fclose(infile);

  return 0;

  error:
  exit(EXIT_FAILURE);
}

void read_input_line(void *A, size_t size, int (*read_number)(void *)) {
  size_t i = 0;
  while(i < size)
    // this line assumes sizeof(long) == sizeof(double)
    check(read_number(A + sizeof(long) * (i++)), "insufficient or malformed data in the input file.");

  // jump to the next line after reading 'size' members
  int c;
  while((c = fgetc(infile)) != '\n' && c != EOF);

  return;

  error:
  abort();
}

int read_integer(void *n) {
  return fscanf(infile, "%ld", ((long *)n));
}

int read_float(void *n) {
  return fscanf(infile, "%lf", ((double *)n));
}

int compare_integer(void *a, void *b) {
  if(*(long *)a > *(long *)b)
    return 1;
  if(*(long *)a == *(long *)b)
    return 0;
  return -1;
}

int compare_float(void *a, void *b) {
  if(*(double *)a > *(double *)b)
    return 1;
  if(*(double *)a == *(double *)b)
    return 0;
  return -1;
}

void print_usage() {
  printf("Usage: sort [options] file...\n");
  printf("Options:\n");
  printf("  -h, --help             \tPrint this help message and exit.\n");
  printf("  -a, --algorithm <[1-7]>\tSorting algorithm to be used.\n");
  printf("  -k, --size <arg>       \tInput data size (2**k).\n");
  printf("Available algorithms for '-a' option are:\n");
  printf("  1 - insertion\n");
  printf("  2 - merge\n");
  printf("  3 - heap\n");
  printf("  4 - quick\n");
  printf("  5 - counting\n");
  printf("  6 - radix\n");
  printf("  7 - bucket\n");
}
