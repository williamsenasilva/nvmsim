/*
 * Emilio Francesquini <e.francesquini@ufabc.edu.br>
 * 2018-10-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ELEM_COUNT 20000

int main() {

  double  *x = malloc(sizeof(double) * ELEM_COUNT);
  double  *y = malloc(sizeof(double) * ELEM_COUNT);
  double **a = malloc(sizeof(double*) * ELEM_COUNT);
  for (int i = 0; i < ELEM_COUNT; ++i) {
    a[i] = malloc(sizeof(double) * ELEM_COUNT);
  }

  /* O uso da varredura das linhas e só então das colunas não
   * privilegia os acessos com localidade espacial. Em lugar de
   * acessar os elementos sequencialmente dentro de uma linha de cache
   * o código abaixo fica pulando de linha em linha desperdiçando os
   * possíveis ganhos promovidos pela localidade espacial.
   */

 clock_t start = clock();
  for (int j = 0; j < ELEM_COUNT; j++) {
    for (int i = 0; i < ELEM_COUNT; ++i) {
      y[i] += a[i][j] * x[j];
    }
  }
  double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
  printf ("JI: %lf\n", elapsedTime);

  return 0;
}
