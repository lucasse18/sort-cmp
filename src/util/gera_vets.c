#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "util/vetor.h"

#define POT2(n) (1 << (n))

FILE *int_out;
FILE *float_out;

void gera_e_salva_vet(int n, Modificador m, Ordem o, Percentual p){
  int *v1 = NULL;
  double *v2 = NULL;

  v1 = gera_vetor_int(n, m, o, p, 1, n);
  escreva_vetor_int(v1, n);

  v2 = gera_vetor_double(n, m, o, p, 1, n);
  escreva_vetor_double(v2, n);

  free(v1);
}

int main(int argc, char *argv[]){
  char diretorio[] = "./data";

  if(argc < 2) {
    fprintf(stderr, "Usage: gen_input <k>\n");
    exit(EXIT_FAILURE);
  }

  int k = atoi(argv[1]);
  CONFIRME(2 < k && k < 21, "invalid input size. values must be in the range [3-20].");
  int n = POT2(k);

  struct stat st = {0};

  if (stat(diretorio, &st) == -1) { // se o diretorio não existir,
    mkdir(diretorio, 0700);         // crie um
  }

  CONFIRME(chdir(diretorio) == 0, "Erro ao mudar de diretório");

  int_out = fopen("input_int", "w");
  float_out = fopen("input_float", "w");

  gera_e_salva_vet(n, TOTALMENTE,   CRESCENTE,   0);
  gera_e_salva_vet(n, TOTALMENTE,   DECRESCENTE, 0);
  gera_e_salva_vet(n, TOTALMENTE,   ALEATORIO,   0);
  gera_e_salva_vet(n, PARCIALMENTE, CRESCENTE,  10);

  fclose(int_out);
  fclose(float_out);

  return 0;
}
