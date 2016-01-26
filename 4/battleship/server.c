#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

static void sighandler( int signo ){

  int i,n,end,win;
  time_t t;

  srand((unsigned) time(&t));
  n = rand() % 3;
  end = 0;

  if (signo == SIGINT){
    if (end == 0){
      printf("You waved the white flag\n");
    if (n == 1){
      printf("What a sore loser!\n");
      printf("You will never bring honor to your family!\n");
	}
    else printf ("It's ok! Try again next time!\n");
    }
    else 
      printf ("You faught well, See you on the battle field next time\n");
    exit(0);
  }
}
int main() {

  signal(SIGINT,sighandler);
  
  int socket_id, socket_client;
  char buffer[256], ship_input[256];
  int hit;
  //create the socket
  socket_id = socket( AF_INET, SOCK_STREAM, 0 );
  
  //bind to port/address
  struct sockaddr_in listener;
  listener.sin_family = AF_INET;  //socket type IPv4
  listener.sin_port = htons(24601); //port #
  listener.sin_addr.s_addr = INADDR_ANY; //bind to any incoming address
  bind(socket_id, (struct sockaddr *)&listener, sizeof(listener));
  
  listen( socket_id, 1 );
  printf("<server> listening\n");

  socket_client = accept( socket_id, NULL, NULL );
  printf("<server> connected: %d\n", socket_client );

  // printf("\n");

  while( 1 ){

  if( socket_client % 2 ) // figure out which player is contacting the server
    write( socket_client, "Player 2", 100 );
  else
    write( socket_client, "Player 1", 100 );

  read( socket_client, buffer, sizeof(buffer) ); // read what the player has sent
  printf( "<server> Client returned: %s\n", buffer );

  // check if what the player has sent is a hit/miss
  //im assuming we can use the board.c functions for this (change truth value of hit)


  // tell them that they have hit/missed
  if ( hit )
    write( socket_client, "You have hit a ship!", 100 );
    //change board
  else 
    write( socket_client, "You have missed!", 100 );
  //server's turn to shoot
  //<NEED SHOOTING FUNCTION>
  
  if ( hit ){
    //TEMP: While there are no shooting function, the ship will always shoot at [4,3]   
    strcpy( ship_input, "You have been hit at [4,3]" );
    write( socket_client, ship_input, 100 ); 
  }
  else{
    //TEMP: While there are no shooting function, the ship will always shoot at [4,3]
    strcpy( ship_input, "You have been missed at [4,3]" );
    write( socket_client, ship_input, 100 );
  }

  }


  return 0;
}
