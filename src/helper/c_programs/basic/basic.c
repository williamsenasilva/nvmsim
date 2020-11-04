#include "stdio.h"
#include <stdlib.h>
#include <time.h>

typedef struct {
    char s[64];
} String64;

void print_data64(String64 *, int);

int main()
{
   int memory_size = 1048576; // (1MB)
   int cache_block_size = 64;
   int total_of_lines = memory_size / cache_block_size;
   int counter_filled_lines = 0;
   int n_random;
   String64 *data64;

   posix_memalign((void **) &data64, 64, total_of_lines * sizeof(String64));

   if(data64 == NULL)
      exit (1);

   // clear data64
   for(int i = 0; i < total_of_lines; i++)
      for (int j = 0; j < cache_block_size; j++)
         data64[i].s[j] = 0x00;

   srand(time(0));

   while(counter_filled_lines != total_of_lines)
   {
      n_random = rand() % total_of_lines;
      if(data64[n_random].s[0] == 0)
      {
         for (int j = 0; j < cache_block_size; j++)
         {
            data64[n_random].s[j] = 0xFF;
         }
         counter_filled_lines++;
      }
   }

   // print_data64(data64, total_of_lines);

   free(data64);

   return 0;
}

void print_data64(String64 *data64, int total_of_lines)
{
   printf("size of String64: %lu\n", sizeof(String64));
   for(int i = 0; i < total_of_lines; i++)
   {
      printf("data64[%d] = ", i);
      for (int j = 0; j < 64; j++)
      {
         printf("%x", data64[i].s[j]);
      }
      printf("\n");
   }
}