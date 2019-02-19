/*
 * Emilio Francesquini <e.francesquini@ufabc.edu.br>
 * 2018-10-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ITERS 100000000


/* Nesta versão otimizada, os threads não foram modificados mas a
 * introdução de um padding na struct garante que as variáveis x e y
 * não ficarão na mesma linha de cache. Logo não ocorre a invalidação
 * destas linhas de maneira mútua por cada núcleo do processador.
 */
struct cordenada {
  int x;
  char PADDING[64]; // Exagerado, pq?
  int y;
};

struct cordenada cord;

void *frita_x (void* ign) {
  int s = 0;
  int i;
  for (i = 0; i < ITERS; i++) {
    cord.x++;
    s += cord.x;
  }
  printf("Soma x: %d\n", s);
  return NULL;
}

void *frita_y (void * ign) {
  int i;
  for (i = 0; i < ITERS; i++)
    cord.y++;
  printf("Soma y: %d\n", cord.y);
  return NULL;
}


int main() {
    pthread_t thread1, thread2;
    clock_t start = clock();
    pthread_create(&thread1, NULL, &frita_x, NULL);
    pthread_create(&thread2, NULL, &frita_y, NULL);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf ("Tempo: %lf\n", elapsedTime);

    return 0;
}
