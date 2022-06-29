#include "stdio.h"
#include <stdlib.h>

#define CACHE_BLOCK_SIZE 64
#define ONE_MEGA 1048576

void clear_cache_lines(int64_t *, int);
void fill_cache_lines(int64_t *, int);
void print_cache_lines(int64_t *, int);
void read_cache_lines(int64_t *, int, int);

int main(int argc, char *argv[])
{
   int memory_size;
   int cache_lines;
   int aleatory_access;
   int64_t *cache_data;
   double timespent;

   if( argc < 3 )
   {
      printf("[ERROR] Args not enough. Please, give 2 args (memory_size aleatory_access)\n");
      printf("[TIP  ] Args use this format example: %s 1 0\n", argv[0]);
      return 1;
   }
   
   memory_size = atoi(argv[1]) * ONE_MEGA;
   aleatory_access = argv[2][0] == '1' ? 1 : 0;
   cache_lines = memory_size / CACHE_BLOCK_SIZE;
   
   posix_memalign((void *) &cache_data, 64, memory_size);

   if (cache_data == NULL)
      exit(1);
   
   // clear_cache_lines(cache_data, cache_lines);
   // print_cache_lines(cache_data, cache_lines);
   fill_cache_lines(cache_data, cache_lines);   
   read_cache_lines(cache_data, cache_lines, aleatory_access);

   printf( "%10s %16s %16s %16s\n", "MODE", "CACHE LINE SIZE", "MEMORY (MB)", "CACHE LINES" );
   printf( "%10s %16d %16d %16d\n", aleatory_access ? "Aleatory" : "Sequential", CACHE_BLOCK_SIZE, memory_size / ONE_MEGA, cache_lines );

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
   int64_t *ptr_index;
   
   for(int i = 0; i < cache_lines; i++)
   {
      ptr_index = &cache_data[i * sizeof(int64_t)];
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
      }
   }
}

void print_cache_lines(int64_t *cache_data, int cache_lines)
{
   int64_t *ptr_index;

   for (int i = 0; i < cache_lines; i++)
   {
      ptr_index = &cache_data[i * sizeof(int64_t)];
      printf("[INFO ] %4d %10p [ ", i + 1, ptr_index);
      for (int j = 0; j < 8; j++)
         printf("%04x ", (unsigned int) *ptr_index++);
      printf("]\n");
   }
}

void read_cache_lines(int64_t *cache_data, int cache_lines, int aleatory_access)
{
   int64_t *ptr_index;
   double timespent;
   unsigned long random_index;
   unsigned long sum = 0;

   for (long index = 0; index < cache_lines; index++)
   {
      random_index = rand() % cache_lines;
      if(aleatory_access)
         ptr_index = &cache_data[random_index * sizeof(int64_t)];
      else
         ptr_index = &cache_data[index * sizeof(int64_t)];
      sum += *ptr_index;
   }

   printf("sum: %lu\n", sum);
}

