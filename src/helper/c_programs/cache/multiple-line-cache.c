#include "stdio.h"
#include <stdlib.h>
#include <time.h>

#define CACHE_BLOCK_SIZE 64

void clear_cache_lines(int64_t *, int);

void fill_cache_lines(int64_t *, int);

void print_cache_lines(int64_t *, int);

int main()
{
   int memory_size = 131072; // 128MB
   int cache_lines = memory_size / CACHE_BLOCK_SIZE;
   int64_t *cache_data;

   posix_memalign((void *) &cache_data, 64, memory_size);

   if (cache_data == NULL)
      exit(1);

   printf("[BASIC ][INFO ][CACHE] memory size: %d\n", memory_size);
   printf("[BASIC ][INFO ][CACHE] cache lines: %d\n", cache_lines);
   printf("[BASIC ][INFO ][CACHE] cache block size: %d\n", CACHE_BLOCK_SIZE);

   clear_cache_lines(cache_data, cache_lines);

   fill_cache_lines(cache_data, cache_lines);

   print_cache_lines(cache_data, cache_lines);

   free(cache_data);

   return 0;
}

void clear_cache_lines(int64_t *cache_data, int cache_lines)
{
   int64_t *ptr_index;
   for (int i = 0; i < cache_lines; i++)
   {
      ptr_index = &cache_data[i * sizeof(int64_t)];
      for (int j = 0; j < 8; j++)
         *ptr_index++ = 0;
   }
}

void fill_cache_lines(int64_t *cache_data, int cache_lines)
{
   int counter_filled_lines = 0;
   int64_t *ptr_index;
   // initialize srand
   srand(time(0));

   while (counter_filled_lines != cache_lines)
   {
      int n_random = rand() % cache_lines;
      ptr_index = &cache_data[n_random * sizeof(int64_t)];
      if (*ptr_index == 0)
      {
         *ptr_index++ = -1;
         *ptr_index++ = -1;
         *ptr_index++ = -1;
         *ptr_index++ = -1;
         *ptr_index++ = -1;
         *ptr_index++ = -1;
         *ptr_index++ = -1;
         *ptr_index = -1;
         counter_filled_lines++;
      }
   }
}

void print_cache_lines(int64_t *cache_data, int cache_lines)
{
   int64_t *ptr_index;
   for (int i = 0; i < cache_lines; i++)
   {
      ptr_index = &cache_data[i * sizeof(int64_t)];
      printf("[BASIC ][INFO ][CACHE] %4d %10p [ ", i + 1, ptr_index);
      for (int j = 0; j < 8; j++)
         printf("%04x ", *ptr_index++);
      printf("]\n");
   }
}
