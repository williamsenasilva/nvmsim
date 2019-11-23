#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define MAX 80 
#define PORT 2000

void chat(int);
int hostname_to_ip(char *, char *);

int main(int argc, char *argv[]) 
{ 
   int client_socket, client_connection; 
   struct sockaddr_in servaddr, cli; 

   if(argc < 2)
   {
      printf("Please provide a hostname to resolve\n");
      exit(1);
   }
    
   char *hostname = argv[1];
   char ip[100];
    
   hostname_to_ip(hostname , ip);
   printf("%s resolved to %s\n" , hostname , ip);

   // socket create and varification 
   client_socket = socket(AF_INET, SOCK_STREAM, 0); 
   if (client_socket == -1) 
   { 
      printf("socket creation failed...\n"); 
      exit(0); 
   } 
   else
   {
      printf("Socket successfully created..\n"); 
   }
   bzero(&servaddr, sizeof(servaddr)); 

   // assign IP, PORT 
   servaddr.sin_family = AF_INET; 
   servaddr.sin_addr.s_addr = inet_addr(ip); 
   servaddr.sin_port = htons(PORT); 

   // connect the client socket to server socket 
   if (connect(client_socket, (struct sockaddr*) &servaddr, sizeof(servaddr)) != 0) 
   { 
      printf("connection with the server failed...\n"); 
      exit(0); 
   } 
   else
   {
      printf("connected to the server..\n"); 
   }

   // function for chat 
   chat(client_socket); 

   // close the socket 
   close(client_socket); 
} 

int hostname_to_ip(char * hostname , char* ip)
{
   struct hostent *he;
   struct in_addr **addr_list;
   int i;
       
   if ( (he = gethostbyname( hostname ) ) == NULL) 
   {
      // get the host info
      herror("gethostbyname");
      return 1;
   }
 
   addr_list = (struct in_addr **) he->h_addr_list;
    
   for(i = 0; addr_list[i] != NULL; i++) 
   {
      //Return the first one;
      strcpy(ip , inet_ntoa(*addr_list[i]) );
      return 0;
   }

   return 1;
}

void chat(int client_socket) 
{ 
   char buffer[MAX]; 
   
   for (int calls = 1; calls <= 100; calls++) 
   { 
      bzero(buffer, sizeof(buffer)); 
      sprintf(buffer, "%d", calls);
      write(client_socket, buffer, sizeof(buffer)); 
      bzero(buffer, sizeof(buffer)); 
      read(client_socket, buffer, sizeof(buffer)); 
      printf("From Server : %s\n", buffer);  
   }

   bzero(buffer, sizeof(buffer)); 
   sprintf(buffer, "%s", "exit");
   write(client_socket, buffer, sizeof(buffer)); 
   printf("Client Exit...\n");
} 