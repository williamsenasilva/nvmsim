/*
 * Emilio Francesquini <e.francesquini@ufabc.edu.br>
 * 2018-10-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ONE_K 1024
#define ONE_M ONE_K * ONE_K
#define ELEM_COUNT 128 * ONE_M

int main() {

  int *a = malloc (ELEM_COUNT * sizeof(int));
  int *b = malloc (ELEM_COUNT * sizeof(int));

  for (int i = 0; i < ELEM_COUNT; i++) {
    a[i] = i;
    b[i] = i;
  }

  /*
   * Com as otimizações do compilador desligadas ele não percebe as
   * chances de utilizar de operações vetoriais nos laços abaixo. Nem
   * sempre o compilador é capaz de perceber que um código é
   * vetorizável. Muitas vezes é, então, possível melhorar o
   * desempenho de códigos otimizados utilizando operações SIMD do
   * processador.
   */
  //Multiplica tudo
  clock_t start = clock();
  for (int i = 0; i < ELEM_COUNT; ++i) {
    a[i] *= 7;
    b[i] *= 3;
  }
  double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
  printf ("Mult: %lf\n", elapsedTime);

  //Soma um a um
  start = clock();
  for (int i = 0; i < ELEM_COUNT; ++i) {
    a[i] += b[i];
  }
  elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
  printf ("Soma: %lf\n", elapsedTime);

  //Imprime inicio do resultado
  for (int i = 0; i < 10; ++i) {
    printf ("a: %d \t b: %d\n", a[i], b[i]);
  }

  return 0;
}
