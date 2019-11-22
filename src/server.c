#include <unistd.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 2000 

void chat(int); 

// Driver function 
int main() 
{ 
   int server_socket, server_connection, len; 
   struct sockaddr_in servaddr, client; 

   // socket create and verification 
   server_socket = socket(AF_INET, SOCK_STREAM, 0); 
   if (server_socket == -1) 
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
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
   servaddr.sin_port = htons(PORT); 

   // Binding newly created socket to given IP and verification 
   if ((bind(server_socket, (struct sockaddr*) &servaddr, sizeof(servaddr))) != 0) { 
      printf("socket bind failed...\n"); 
      exit(0); 
   } 
   else
      printf("Socket successfully binded..\n"); 

   // Now server is ready to listen and verification 
   if ((listen(server_socket, 5)) != 0) 
   { 
      printf("Listen failed...\n"); 
      exit(0); 
   } 
   else
   {
      printf("Server listening..\n"); 
   }
   len = sizeof(client); 

   // Accept the data packet from client and verification 
   server_connection = accept(server_socket, (struct sockaddr*) &client, &len); 
   if (server_connection < 0) 
   { 
      printf("server acccept failed...\n"); 
      exit(0); 
   } 
   else
   {
      printf("server acccept the client...\n"); 
   }

   // Function for chatting between client and server 
   chat(server_connection); 

   // After chatting close the socket 
   close(server_socket); 
} 

// Function designed for chat between client and server. 
void chat(int server_connection) 
{ 
   char buffer[MAX]; 
   int pow;
   for(;;) 
   { 
      bzero(buffer, sizeof(buffer)); 
      read(server_connection, buffer, sizeof(buffer)); 
      printf("From client: %s\n", buffer);
      
      // if msg contains "Exit" then server exit and chat ended. 
      if (strncmp("exit", buffer, 4) == 0) 
      { 
         printf("Server Exit...\n"); 
         break; 
      }

      pow = atoi(buffer) * atoi(buffer);
      sprintf(buffer, "%d", pow);
      write(server_connection, buffer, sizeof(buffer));  
   } 
}