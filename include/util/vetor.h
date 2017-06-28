#ifndef _VETOR_H
#define _VETOR_H

#include <stdbool.h>

#define CONFIRME(teste,msg) (                   \
    {if (!(teste)){                             \
      fprintf(stderr, (msg));                   \
      exit(-1);}})


typedef enum ordem {ALEATORIO, CRESCENTE, DECRESCENTE} Ordem;
typedef enum modificador {TOTALMENTE, PARCIALMENTE} Modificador;
typedef int Percentual;


/*
 * Função: gera_vetor_int
 * ----------------------
 * Aloca e preenche um vetor de inteiros com a especificação fornecida.
 *
 * n - número de elementos no vetor.
 * c - modificador do vetor:
 *     TOTALMENTE: nenhuma mudança será aplicado ao ordenamento do vetor.
 *     PARCIALMENTE: parte dos elementos do vetor serão mudados de lugar e
 *                   nesse caso, o percentual de mudanças deve ser informado.
 * o - ordenamento do vetor:
 *     ALEATORIO: um vetor com elementos escolhidos aleatoriamente.
 *     CRESCENTE: um vetor com elementos em ordem crescente.
 *     DECRESCENTE: um vetor com elementos em ordem decrescente.
 * p - um percentual inteiro entre 0 e 100 de modificação do vetor.
 * minimo - o menor valor inteiro de um elemento no vetor.
 * maximo - o maior valor inteiro de um elemento no vetor.
 *
 * SAÍDA o vetor de inteiros v[0..n-1] gerado
 *
 * OBSERVAÇÂO: liberar a memória quando o vetor não for mais necessário:
 *             free(v);
 *
 * Exemplos de uso:
 *  - vetor com 10 elementos inteiros totalmente ordenado em ordem crescente,
 *    com o menor valor igual a 10 e o maior igual a 19:
 *       int * v;
 *       v = gera_vetor_int(10, TOTALMENTE, CRESCENTE, 1, 10, 19);
 *
 *  - vetor com 32 elementos inteiros totalmente ordenado em ordem decrescente,
 *    com o menor valor igual a 5 e o maior igual a 25. Note que nesse haverá
 *    repetição de elementos:
  *      int * v;
 *       v = gera_vetor_int(32, TOTALMENTE, DECRESCENTE, 1, 5, 25);
 *
 *  - vetor com 50 elementos inteiros escolhidos aleatoriamente,
 *    com o menor valor igual a 10 e o maior igual a 19:
  *      int * v;
 *       v = gera_vetor_int(50, TOTALMENTE, ALEATORIO, 1, 10, 19);
 *
 *  - vetor com 100 elementos inteiros parcialmente ordenado em ordem
 *    crescente, com o menor valor igual a 10 e o maior igual a 50. Aqui
 *    20% dos elementos estarão fora de ordem:
 *       int * v;
 *       v = gera_vetor_int(100, PARCIALMENTE, CRESCENTE, 20, 10, 50);
 *
 */
int * gera_vetor_int(int n, Modificador c, Ordem o, Percentual p,
                     int minimo, int maximo);

/*
 * Função: gera_vetor_double
 * -------------------------
 * Aloca e preenche um vetor de doubles com a especificação fornecida.
 *
 * n - número de elementos no vetor.
 * c - modificador do vetor:
 *     TOTALMENTE: nenhuma mudança será aplicado ao ordenamento do vetor.
 *     PARCIALMENTE: parte dos elementos do vetor serão mudados de lugar e
 *                   nesse caso, o percentual de mudanças deve ser informado.
 * o - ordenamento do vetor:
 *     ALEATORIO: um vetor com elementos escolhidos aleatoriamente.
 *     CRESCENTE: um vetor com elementos em ordem crescente.
 *     DECRESCENTE: um vetor com elementos em ordem decrescente.
 * p - um percentual inteiro entre 0 e 100 de modificação do vetor.
 * minimo - o menor valor em ponto flutuante de um elemento no vetor.
 * maximo - o maior valor em ponto flutuante de um elemento no vetor.
 *
 * SAÍDA o vetor de doubles v[0..n-1] gerado
 *
 * OBSERVAÇÂO: liberar a memória quando o vetor não for mais necessário:
 *             free(v);
 *
 * Exemplos de uso:
 *  - vetor com 10 elementos doubles totalmente ordenado em ordem crescente,
 *    com o menor valor igual a 10.0 e o maior igual a 15.0:
 *       double * v;
 *       v = gera_vetor_double(10, TOTALMENTE, CRESCENTE, 1, 10.0, 15.0);
 *
 *  - vetor com 32 elementos doubles totalmente ordenado em ordem decrescente,
 *    com o menor valor igual a 5.0 e o maior igual a 25. Note que nesse haverá
 *    repetição de elementos:
 *       double * v;
 *       v = gera_vetor_double(32, TOTALMENTE, DECRESCENTE, 1, 5.0, 25.0);
 *
 *  - vetor com 50 elementos doubles escolhidos aleatoriamente,
 *    com o menor valor igual a 10.0 e o maior igual a 20.0:
 *       double * v;
 *       v = gera_vetor_double(50, TOTALMENTE, ALEATORIO, 1, 10.0, 20.0);
 *
 *  - vetor com 100 elementos doubles parcialmente ordenado em ordem
 *    crescente, com o menor valor igual a 10.0 e o maior igual a 50.0. Aqui
 *    20% dos elementos estarão fora de ordem:
 *       double * v;
 *       v = gera_vetor_double(100, PARCIALMENTE, CRESCENTE, 20, 10, 50);
*/
double * gera_vetor_double(int n, Modificador c, Ordem o, Percentual p,
                           double minimo, double maximo);

/*
 * Função: escreva_vetor_int
 * -------------------------
 * Escreve um vetor de inteiros para um arquivo em formato texto.
 *
 * v: o vetor de inteiros
 * n: o número de elementos de v
 * arq: o nome do arquivo para onde o vetor será escrito.
 *
 * SAÍDA: não há
 *
 * OBSERVAÇÂO: a primeira linha do arquivo conterá o número n de elementos de v.
 *
 * Exemplo de uso:
 *
 *   int * v;
 *   v = gera_vetor_int(100, PARCIALMENTE, CRESCENTE, 20, 10, 50);
 *   escreva_vetor_int(v, 100, "vIntCrescente_100_P20.dat");
 *   free(v);
 */
void escreva_vetor_int(int * v, int n);

/*
 * Função: escreva_vetor_double
 * -------------------------
 * Escreve um vetor de doubles para um arquivo em formato texto.
 *
 * v: o vetor de doubles
 * n: o número de elementos de v
 * arq: o nome do arquivo para onde o vetor será escrito.
 *
 * SAÍDA: não há
 *
 * OBSERVAÇÂO: a primeira linha do arquivo conterá o número n de elementos de v.
 *
 * Exemplo de uso:
 *
 *   double * v;
 *   v = gera_vetor_double(100, TOTALMENTE, ALEATORIO, 1, 10, 50);
 *   escreva_vetor_double(v, 100, "vRealAleatorio_100.dat");
 *   free(v);
 */
void escreva_vetor_double(double * v, int n);

/*
 * Função: leia_vetor_int
 * ----------------------
 * Lê um vetor de inteiros a partir de um arquivo.
 *
 * arq: nome do arquivo contendo o vetor de inteiros.
 * n : variável que conterá o número de elementos lidos
 *
 * SAÍDA: o vetor de inteiros v[0..n-1]
 *        o número n de elementos lidos
 *
 * OBSERVAÇÂO: liberar a memória quando o vetor não for mais necessário:
 *             free(v);
 *
 * Exemplo de uso:
 *
 *   int * v;
 *   int * w;
 *   int n;
 *   v = gera_vetor_int(100, PARCIALMENTE, CRESCENTE, 20, 10, 50);
 *   escreva_vetor_int(v, 100, "vIntCrescente_100_P20.dat");
 *
 *   w = leia_vetor_int("vIntCrescente_100_P20.dat", &n);
 *   free(v);
 *   free(w);
 */
int * leia_vetor_int(char * arq, int * n);

/*
 * Função: leia_vetor_double
 * -------------------------
 * Lê um vetor de doubles a partir de um arquivo.
 *
 * arq: nome do arquivo contendo o vetor de doubles.
 * n : variável que conterá o número de elementos lidos
 *
 * SAÍDA: o vetor de doubles v[0..n-1]
 *        o número n de elementos lidos
 *
 * OBSERVAÇÂO: liberar a memória quando o vetor não for mais necessário:
 *             free(v);
 *
 * Exemplo de uso:
 *
 *   double * v;
 *   double * w;
 *   int n;
 *   v = gera_vetor_double(100, PARCIALMENTE, CRESCENTE, 20, 10, 50);
 *   escreva_vetor_double(v, 100, "vRealCrescente_100_P20.dat");
 *
 *   w = leia_vetor_double("vRealCrescente_100_P20.dat", &n);
 *   free(v);
 *   free(w);
 */
double * leia_vetor_double(char * arq, int * n);


/*
 * Função: esta_ordenado_int
 * ------------------------
 * Verifica se um vetor de inteiros está ordenado.
 *
 * o - ordenamento do vetor:
 *     CRESCENTE: um vetor com elementos em ordem crescente.
 *     DECRESCENTE: um vetor com elementos em ordem decrescente.
 * v - o vetor de inteiros a ser verificado.
 * n - o número de elementos de v.
 *
 * SAÍDA: true, se o vetor está na ordem indicada;
 *        false, em caso contrário.
 *
 */
bool esta_ordenado_int(Ordem o, int * v, int n);


/*
 * Função: esta_ordenado_double
 * ------------------------
 * Verifica se um vetor de doubles está ordenado.
 *
 * o - ordenamento do vetor:
 *     CRESCENTE: um vetor com elementos em ordem crescente.
 *     DECRESCENTE: um vetor com elementos em ordem decrescente.
 * v - o vetor de doubles a ser verificado.
 * n - o número de elementos de v.
 *
 * SAÍDA: true, se o vetor está na ordem indicada;
 *        false, em caso contrário.
 *
 */
bool esta_ordenado_double(Ordem o, double * v, int n);


/*
 * Função: imprime_vetor_int
 * -------------------------
 * Imprime na tela um vetor de inteiros.
 *
 * v - o vetor de inteiros a ser verificado.
 * n - o número de elementos de v.
 *
 * SAÍDA: não há
 */
void imprime_vetor_int(int * v, int n);


/*
 * Função: imprime_vetor_double
 * ----------------------------
 * Imprime na tela um vetor de doubles.
 *
 * v - o vetor de doubles a ser verificado.
 * n - o número de elementos de v.
 *
 * SAÍDA: não há
 */
void imprime_vetor_double(double * v, int n);


#endif//_VETOR_H
