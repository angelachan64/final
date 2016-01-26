#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {

  int socket_id, socket_server;
  char buffer[256];
  int i;
  char user_input[256];

  //create the socket
  socket_id = socket( AF_INET, SOCK_STREAM, 0 );
  
  //bind to port/address
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;   
  sock.sin_port = htons(24601);
  //Set the IP address to connect to
  //127.0.0.1 is the "loopback" address of any machine
  inet_aton( "127.0.0.1", &(sock.sin_addr) );
  bind( socket_id, (struct sockaddr *)&sock, sizeof(sock));
  
  //attempt a connection
  socket_server = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("<client> connect returned: %d\n", socket_server);

  

  //handshake stuff
  read( socket_id, buffer, sizeof(buffer));
  printf("<client> received: [%s]\n", buffer );

  while(1){
    //user input
    printf( "Enter Your Coordinates: \n" );
    fgets(user_input, sizeof(user_input), stdin);
    
    write( socket_id, user_input, sizeof(user_input) );
    //printf( "%d\n", i);

    read( socket_id, buffer, sizeof(buffer) );
    printf("<client> received: [%s]\n", buffer);
    
    write(socket_id, "hi", 4);
    
    //server's turn to shoot
    printf("Server is thinking... \n");
    wait(2);
    read( socket_id, buffer, sizeof(buffer) );
    printf("<client> recieved: [%s]\n", buffer); //should say where it shot, and if player was hit
    
  }
  return 0;
}
