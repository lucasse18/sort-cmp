#include <stdlib.h>
#include <stdio.h>

#include "util/vetor.h"

#define MAX(x,y) ( \
    { __auto_type __x = (x); __auto_type __y = (y); \
      __x > __y ? __x : __y; })

#define TROCA(v, i, j, temp) (  \
    { (temp) = v[(i)];     \
      v[(i)] = v[(j)];     \
      v[(j)] = (temp);})

extern FILE *int_out;
extern FILE *float_out;

/*
typedef enum ordem {ALEATORIO, CRESCENTE, DECRESCENTE} Ordem;
typedef enum modificador {TOTALMENTE, PARCIALMENTE} Modificador;
typedef int Percentual;
*/

double rand_double(double min, double max)
{ // Retorna números em ponto flutuante aleatórios uniformemente
  // distribuídos no intervalo fechado [min,max].
  return min + (rand() / (RAND_MAX / (max-min)));
}

int rand_int(int min, int max){
  // Retorna números inteiros aleatórios uniformemente distribuídos
  // no intervalo fechado [min,max].
  // Para maiores informações:
  // https://stackoverflow.com/questions/2509679/how-to-generate-a-random-number-from-within-a-range
  unsigned long num_baldes = (unsigned long) max-min+1;
  if (num_baldes<1){
    fprintf(stderr, "Intervalo invalido\n");
    exit(-1);
  }
  unsigned long num_rand  = (unsigned long) RAND_MAX+1;
  unsigned long tam_balde = num_rand / num_baldes;
  unsigned long defeito   = num_rand % num_baldes;
  long x;
  do
    x = random();
  while (num_rand - defeito <= (unsigned long)x);
  return x / tam_balde + min;
}


static void inline preenche_vetor_int(int * v, int n, int k, int q, int r, int incr){
  int i, j;
  i=0;
  while (i < n) {
    for(j=i; j < i+q; j++)
      v[j] = k;

    i = i + q;
    if (r > 0) {
      v[j] = k;
      i = i + 1;
      r = r - 1;
    }
    k = k + incr;
  }
}


int * gera_vetor_int(int n, Modificador c, Ordem o, Percentual p,
                     int minimo, int maximo){
  int i; // índices
  int a = maximo - minimo + 1; // amplitude do intervalo
  int q = n / a;  // número mínimo de valores repetidos
  int r = n % a;  // r elementos terão o número (q+1) valores repetidos
  int k;          // valor do elemento atualmente sob consideração
  int * v;        // vetor[0..n-1] a ser preenchido

  CONFIRME(n >= 1, "O número de elementos deve ser estritamente positivo.\n");
  CONFIRME(maximo >= minimo, "O valor máximo deve ser maior que o mínimo.\n");
  CONFIRME(0 <= p && p <= 100, "O percentual deve estar entre [0,100]\n");

  v = (int *) calloc(n, sizeof(int)); // aloca um vetor com n inteiros
  CONFIRME(v != NULL, "calloc falhou\n");

  switch (o) {
    case CRESCENTE:
      preenche_vetor_int(v, n, minimo, q, r, 1);
      break;
    case DECRESCENTE:
      preenche_vetor_int(v, n, maximo, q, r, -1);
      break;
    case ALEATORIO:
      for(i=0; i<n; i++) v[i] = rand_int(minimo,maximo);
      break;
    default: CONFIRME(false, "Ordem Inválida\n");
  }

  switch (c) {
    case PARCIALMENTE:
      q = (p * n) / 200;
      for(i=0;i<q;i++)
        TROCA(v,i,n-i-1,k);
      break;
    case TOTALMENTE: break;
    default: CONFIRME(false, "Modificador do vetor desconhecido");
  }

  return v;
}


static void inline preenche_vetor_double(double * v, int n, double inicial,
                                         double delta, double sinal)
{
  int i;
  for(i=0;i<n;i++)
    v[i]= inicial + sinal*i*delta;
}


double * gera_vetor_double(int n, Modificador c, Ordem o, Percentual p,
                           double minimo, double maximo){
  int i; // índice
  double a = maximo - minimo;    // amplitude do intervalo
  double delta;
  double * v;       // vetor[0..n-1] a ser preenchido
  double temp;
  int q;

  CONFIRME(n >= 1, "O número de elementos deve ser estritamente positivo.\n");
  CONFIRME(maximo >= minimo, "O valor máximo deve ser maior que o mínimo.\n");
  CONFIRME(0 <= p && p <= 100, "O percentual deve estar entre [0,100]\n");

  delta =  a / MAX(n-1.0, 1.0);   // incremento nos elementos do vetor
  v = (double *) calloc(n, sizeof(double)); // aloca um vetor com n doubles
  CONFIRME(v != NULL, "calloc falhou\n");

  switch (o) {
    case CRESCENTE:
      preenche_vetor_double(v, n, minimo, delta, 1);
      break;
    case DECRESCENTE:
      preenche_vetor_double(v, n, maximo, delta, -1);
      break;
    case ALEATORIO:
      for(i=0; i<n; i++) v[i] = rand_double(minimo,maximo);
      break;
    default: CONFIRME(false, "Ordem Inválida\n");
  }

  switch (c) {
    case PARCIALMENTE:
      q = (p * n) / 200;
      for(i=0;i<q;i++)
        TROCA(v,i,n-i-1,temp);
      break;
    case TOTALMENTE: break;
    default: CONFIRME(false, "Modificador do vetor desconhecido");
  }

  return v;
}

void escreva_vetor_int(int * v, int n){
  int i;

  for(i=0; i<n; i++)
    fprintf(int_out, "%d ", v[i]);
  fprintf(int_out, "\n");
}

void escreva_vetor_double(double * v, int n){
  int i;

  for(i=0; i<n; i++)
    fprintf(float_out, "%f ", v[i]);
  fprintf(float_out, "\n");
}

int * leia_vetor_int(char * arq, int * n){
  int i;
  FILE* fd = NULL;
  int * v;

  fd = fopen(arq, "r");
  CONFIRME(fd!= NULL, "leia_vetor_int: fopen falhou\n");

  // Leia o número de elementos do vetor
  CONFIRME(fscanf(fd, "%d\n", n) == 1,
           "leia_vetor_int: erro ao ler o número de elementos do vetor\n");

  v = (int *) calloc(*n, sizeof(int)); // aloca um vetor com n inteiros
  CONFIRME(v != NULL, "leia_vetor_int: calloc falhou\n");

  i=0;
  while(fscanf(fd, "%d\n", &v[i]) == 1) i++;
  fclose(fd);

  return v;
}

double * leia_vetor_double(char * arq, int * n){
  int i;
  FILE* fd = NULL;
  double * v;

  fd = fopen(arq, "r");
  CONFIRME(fd!= NULL, "leia_vetor_int: fopen falhou\n");

  // Leia o número de elementos do vetor
  CONFIRME(fscanf(fd, "%d\n", n) == 1,
           "leia_vetor_double: erro ao ler o número de elementos do vetor\n");


  // Aloca um vetor com n doubles
  v = (double *) calloc(*n, sizeof(double));
  CONFIRME(v != NULL, "leia_vetor_int: calloc falhou\n");

  i=0;
  while(fscanf(fd, "%lf\n", &v[i]) == 1) i++;
  fclose(fd);

  return v;
}

bool esta_ordenado_int(Ordem o, int * v, int n){
  int i;

  CONFIRME(n > 0,
           "estaOrdenado_int: o número de elementos deve ser maior que zero.\n");
  if (n == 1) return true;
  switch (o) {
    case CRESCENTE:
      for(i=0;i<n;i++)
        if (v[i-1] > v[i])
          return false;
      break;
    case DECRESCENTE:
      for(i=0;i<n;i++)
        if (v[i-1] < v[i])
          return false;
      break;
    default: CONFIRME(false, "estaOrdenado_int: Ordem Inválida\n");
  }
  return true;
}


bool esta_ordenado_double(Ordem o, double * v, int n){
  int i;

  CONFIRME(n > 0,
           "estaOrdenado_double: o número de elementos deve ser maior que zero.\n");
  if (n == 1) return true;
  switch (o) {
    case CRESCENTE:
      for(i=0;i<n;i++)
        if (v[i-1] > v[i])
          return false;
      break;
    case DECRESCENTE:
      for(i=0;i<n;i++)
        if (v[i-1] < v[i])
          return false;
      break;
    default: CONFIRME(false, "estaOrdenado_double: Ordem Inválida\n");
  }
  return true;
}

void imprime_vetor_int(int * v, int n){
  int i;

  for(i=0; i < n; i++)
    printf("v[%d] = %d\n", i, v[i]);
  printf("\n");
}

void imprime_vetor_double(double * v, int n){
  int i;

  for(i=0; i < n; i++)
    printf("v[%d] = %lf\n", i, v[i]);
  printf("\n");
}
