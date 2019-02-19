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

// Define um tipo vetorial de 4 inteiros
typedef int v4si __attribute__ ((vector_size (16)));

// Macro para auxiliar o acesso à posição I do vetor V de inteiros
// do tipo v4si definido acima
#define VAT(V, I) (V[I >> 2][I & 3])

int main() {

  int n_vets = ELEM_COUNT / 4;
  v4si *a = malloc (n_vets  * sizeof(v4si));
  v4si *b = malloc (n_vets  * sizeof(v4si));

  for (int i = 0; i < ELEM_COUNT; i++) {
    VAT(a, i) = i;
    VAT(b, i) = i;
  }


  /* Como agora estamos utilizando operações vetoriais, cada uma das
   * operações é, na verdade, feita em 4 inteiros por vez. O loop,
   * portanto, tem 4 x menos iterações a percorer (n_vets =
   * ELEM_COUNT/4)
   */
  //Multiplica tudo
  clock_t start = clock();
  for (int i = 0; i < n_vets; ++i) {
    a[i] *= 7;
    b[i] *= 3;
  }
  double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
  printf ("Mult: %lf\n", elapsedTime);

  //Soma um a um
  start = clock();
  for (int i = 0; i < n_vets; ++i) {
    a[i] += b[i];
  }
  elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
  printf ("Soma: %lf\n", elapsedTime);

  //Imprime inicio do resultado
  for (int i = 0; i < 10; ++i) {
    printf ("a: %d \t b: %d\n", VAT(a, i), VAT(b, i));
  }

  return 0;
}
