/*
 * 1
 * reserva através de mmap um espaço/página de N bytes na memória
 * obs: inicialmente neste espaço haverá lixo, mas após a primeira leitura o processador carregará
 *      os dados na memória dentro deste espaço. Nas próximas leituras os dados já estarão na memória
 * 
 * 2
 * o espaço paginado é preenchido com caracteres printaveis da tabela ASCII
 * isto ajuda a ver que os dados foram escritos e também utilizar estes valores
 * como inteiros para somá-los. Somar estes valores impede qualquer otmização que ignore a leitura
 * de dados não lidos, mas não utilizados
 * 
 * 3
 * no caso aleatório, realiza leituras aleatórias para driblar o cache prefetching
 * assim o processador é forçado a ficar buscando os dados que não estão na cache porque não couberam
 */

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#define ONE_MEGA 1048576
#define MICRO ((unsigned long) 1000000)
#define NANOS ((unsigned long)1000000000)

void clear_page(char *page, long memory_size);
void fill_page(char *page, long memory_size, double *timespent);
void print_page(char *page, long memor_size, double *timespent);
void read_page(char *page, long memor_size, int aleatory_access, double *timespent);

int main(int argc, char *argv[]) {
   int flags, protect, file_descriptor, offset, aleatory_access;
   long memory_size;
   clock_t time_ini, time_end;
   double timespent_to_read, timespent_to_write;
   char *filepath;
   
   if( argc < 3)
   {
      printf("[ERROR] Args not enough. Please, give at least 2 args (memory_size aleatory_access)\n");
      printf("[TIP  ] Args use this format example: %s 1 0 filepath\n", argv[0]);
      return 1;
   }

   memory_size = (long) atoi( argv[1] ) * ONE_MEGA;
   aleatory_access = argv[2][0] == '1' ? 1 : 0;
   filepath = argv[3];

   protect = PROT_READ | PROT_WRITE;
   offset = 0;
   
   // se tem filepath, cria file descriptor no disco, do contrário cria na memória
   if ( argc == 4 ) 
   {
      FILE *file = fopen( filepath, "w" );
      if( file == NULL ) {
         printf( "[ERROR] error on create %s\n", filepath );
         return 1;
      }
      fseek( file, memory_size - 1 , SEEK_SET );
      fputc( EOF, file );
      fclose( file );
      
      flags = MAP_SHARED | MAP_FILE;
      file_descriptor = open(filepath, O_RDWR, 0);
   }
   else
   {
      flags = MAP_PRIVATE | MAP_ANONYMOUS;
      file_descriptor = 0;
   }
   
   // aloca uma página de tamanho memory_size bytes na memória ou disco
   char *page = (char *) mmap( NULL, memory_size * sizeof( char ), protect, flags, file_descriptor, offset );
   if( page == MAP_FAILED )
   {
      printf("[ERROR] mmap failed (MAP_FAILED)\n");
      return 1;
   }
   
   // preenche espaço na memória ou disco com caracteres printaveis da tabela ASCII
   fill_page(page, memory_size, &timespent_to_write);
   
   // realiza leituras na memória ou disco
   read_page(page, memory_size, aleatory_access, &timespent_to_read);
   
   printf( "%10s %25s %25s %25s %25s\n", "MODE", "MEMORY (MB)", "TIME SPENT READ (s)", "TIME SPENT WRITE (s)", "LOCAL" );
   printf( "%10s %25lu %25.2lf %25.2lf %25s\n", 
         aleatory_access ? "Aleatory" : "Sequential", 
         memory_size / ONE_MEGA, 
         timespent_to_read, 
         timespent_to_write,
         file_descriptor ? filepath : "Memory"
         );
   
   /* libera o espaço para uso de outros programas */
   int err = munmap( page, memory_size * sizeof( char ) );
   if( err != 0 )
   {
      printf( "[ERROR] munmap failed\n" );
      return 1;
   }
   
   return 0;
} /* end of main */

void clear_page(char *page, long memory_size)
{
   for( long i = 0; i < memory_size; i++ )
      page[i] = 0;
}

void fill_page(char *page, long memory_size, double *timespent)
{
   struct timespec tick, tock;
   
   clock_gettime(CLOCK_REALTIME, &tick);
   for( long i = 0; i < memory_size; i++ )
      page[i] = i % 127 > 32 ? i % 127 : 33;
   clock_gettime(CLOCK_REALTIME, &tock);
   *timespent = (double)(NANOS * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec) / NANOS;
}

void print_page(char *page, long memory_size, double *timespent)
{
   unsigned long sum = 0;
   clock_t time_ini, time_end;
   
   time_ini = clock();
   for( long index = 0; index < memory_size; index++ )
   {
      printf("%c", page[index]);
      sum += page[index];
   }
   time_end = clock();
   *timespent = ( ( double ) (time_end - time_ini) ) / CLOCKS_PER_SEC;
   
   printf("[INFO ] sum: %lu\n", sum);
}

void read_page(char *page, long memory_size, int aleatory_access, double *timespent)
{
   unsigned long sum = 0;
   unsigned long random_index;
   struct timespec tick, tock;
   
   srand( time( NULL ) );
   
   clock_gettime(CLOCK_REALTIME, &tick);
   for( long index = 0; index < memory_size; index++ )
   {
      random_index = rand() % memory_size;
      if(aleatory_access)
         sum += page[random_index];
      else
         sum += page[index];
   }
   clock_gettime(CLOCK_REALTIME, &tock);
   *timespent = (double)(NANOS * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec) / NANOS;
   
   printf("[INFO] sum: %lu\n", sum);
}

