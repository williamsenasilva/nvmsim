/*
 * Emilio Francesquini <e.francesquini@ufabc.edu.br>
 * 2018-10-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ONE_K 1024
#define ONE_M ONE_K * ONE_K

#define VSIZE (1 * 100000)
#define ITERS (1 * 1000)

int main() {

  srand(time(NULL));

  //Vetor com 100 MB
  unsigned char *vec = malloc (VSIZE);
  for (int i = 0; i < VSIZE; i++) {
    vec[i] = rand() % 256;
  }

  /* O fato do acesso à memória ser aleatório causa problemas de mal
   * uso da cache (falta localidade espacial e causa problemas ao
   * prefecher)
   */
  clock_t start = clock();
  unsigned int sum = 0;
  for (int i = 0; i < ITERS; i++) {
    int pos = rand() % VSIZE;
    if (pos > VSIZE)
      printf("Impossivel. Apenas para forçar a manter pos.");
    else
      sum += vec[pos];
  }
  double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
  printf ("Tempo total: %lf\n", elapsedTime);

  return 0;
}
