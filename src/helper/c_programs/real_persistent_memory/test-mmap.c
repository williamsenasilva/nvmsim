#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#define ONE_MEGA 1048576

/*
NAME
   mmap, munmap - map or unmap files or devices into memory
SYNOPSIS
   #include <sys/mman.h>

   void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
   int munmap(void *addr, size_t length);
*/

int main(int argc, char *argv[]) {
   int flags, protect, file_descriptor, offset;
   long length;
   unsigned long sum = 0;
   clock_t time_ini, time_end;
   double time_spent;
     
   if( argc < 2 ) 
   {
      printf("[ERROR] args not enough\n");
      return 1;
   }
   
   length = atoi( argv[1] ) * ONE_MEGA;
   protect = PROT_READ | PROT_WRITE;
   offset = 0;
   
   if ( argc == 2 ) {
      flags = MAP_PRIVATE | MAP_ANONYMOUS;
      file_descriptor = 0;
   }
   else {
      char *filename = argv[2];
      FILE *file = fopen( filename, "w" );
      if( file == NULL ) {
         printf( "[ERROR] error on create %s\n", filename );
         return 1;
      }
      fseek( file, length - 1 , SEEK_SET );
      fputc( EOF, file );
      fclose( file );

      flags = MAP_SHARED | MAP_FILE;
      file_descriptor = open(filename, O_RDWR, 0);
   }

   /*
   * reserva um espaço/página de N bytes na memória
   * inicialmente neste espaço haverá lixo
   * após a primeira leitura o processador carregará os dados na memória dentro deste espaço
   * nas próximas leituras os dados já estarão na memória 
   */
   char *page = mmap( NULL, length * sizeof(char), protect, flags, file_descriptor, offset );
   if( page == MAP_FAILED )
   {
      printf("[ERROR] mmap failed (MAP_FAILED)\n");
      return 1;
   }

   /* 
   * escrevo os caracteres printaveis da tabela ASCII 
   * necessário para certificar que irei ler meus dados ao invés de lixo 
   */
   for( int i = 0; i < length; i++ ) 
      page[i] = i % 127 > 32 ? i % 127 : 33;

   /*
   * realizo leituras aleatórias para driblar o cache prefetching     
   * forço o processador a ficar buscando os dados que não estão na cache porque não couberam 
   */
   srand(time(NULL));
   time_ini = clock();
   for( long i = 0; i < length; i++ )
   {
      long random_index = rand() % length;
      sum += page[random_index];
      //printf( "[%lu/%lu] = %c\n", random_index, length, page[random_index] );
   }
   time_end = clock();
   time_spent = ((double) (time_end - time_ini)) / CLOCKS_PER_SEC;
   printf( "[INFO ] size memory: %luM | time spent: %.2fs | sum: %lu\n", length / ONE_MEGA, time_spent, sum );

   /* libero o espaço para uso de outros programas */
   int err = munmap( page, length * sizeof(char));
   if( err != 0 )
   {
      printf( "[ERROR] munmap failed\n" );
      return 1;
   }
   
   return 0;
} /* end of main */

