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

  /* A simples inversão na ordem dos loops fazem com que os acessos
   * ocorram sequencialmente dentro de uma linha de cache.
   */
  clock_t start = clock();
  for (int i = 0; i < ELEM_COUNT; ++i) {
    for (int j = 0; j < ELEM_COUNT; j++) {
      y[i] += a[i][j] * x[j];
    }
  }
  double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
  printf ("IJ: %lf\n", elapsedTime);

  return 0;
}
