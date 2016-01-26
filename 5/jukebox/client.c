#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client.h"
#include "player.h"

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

void parse(char * command, int socket_id){
  char * command_copy = malloc(strlen(command));
  strcpy(command_copy, command);
  command = strsep(&command, " ");
  if(command_copy[0] == 'p'){ //play cmd goes here
	printf("boutta play\n");
	send_to_server(command_copy, socket_id);
	play_song(socket_id);
	}	
  else if(command_copy[0] == 'q'){
		printf("boutta quit\n");
		send_to_server(command_copy, socket_id);
		exit(1);
    }
  else if(command_copy[0] == 'l'){
      printf("boutta list\n");
      send_to_server(command_copy, socket_id);
      list_songs(socket_id);
    }
}


void send_to_server(char * message, int socket_id){
	//printf("message: [%s] \n", message);
	//printf("socket_id: %d\n", socket_id);
  if ((write(socket_id,message, strlen(message)))== -1) {
    fprintf(stderr, "Failure Sending Message\n");
    printf("errno : %s \n", strerror(errno));
    close(socket_id);
    exit(1);
    }
}

void list_songs(int socket_id){
	printf("in th list fun\n");
  char songs[1024];
  if (read(socket_id, songs, sizeof(songs)) < 0 ){
  	printf("unable to read songs\n");
  }
  else {
  	printf("Client:Song List Received From Server -  \n%s\n", songs);
  }
}




void play_song(int socket_id){
  void * mp3 = calloc(1, 1048576); // 1mb at a time
  int song_file;
  song_file = open("temp.mp3", O_CREAT | O_RDWR | O_TRUNC | O_APPEND , 0644);
  // if (recv(socket_id, mp3, 1048576 * 10, 0) < 0){
  // 	printf("erro readin\n");
  // }
  int read_ret;
  printf("ready to roll and read dat song\n");
  while ((read_ret = read(socket_id, mp3, 1048576)) > 0){
  	printf("read ret: %d\n", read_ret);
  	printf("readin\n");
  	if (read_ret > 0){
		write(song_file, mp3, 1048576);
	}
	printf("will I read again?\n");
  }
  printf("left dat loop\n");
  //printf("Client:Message Received From Server -  %s\n", mp3);
  if (strcmp(mp3, "-1") == 0){
  	printf("unable to play the song\n");
  	return;
  }
  printf("gonna play now?\n");
  playsong();
  //playsong code
  //  recieve mp3 file, save it to a temp file, play, delete it from the temp file
}

int main(int argc, char **argv) {
  int socket_id;
  char buffer[256];
  int i;
  socket_id = socket( AF_INET, SOCK_STREAM, 0 );
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;   
  sock.sin_port = htons(24601);
  inet_aton( "127.0.0.1", &(sock.sin_addr) );
  bind( socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("errno: %s\n", strerror(errno));
  i = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  if (i < 0 ){
  	printf("error connecting, quitting\n");
  	exit(0);
  }
  printf("<client> connect returned: %d\n", i);
  printf("errno: %s\n", strerror(errno));
  char input[256];
  while(1){
    printf("type p followed by a songId to play that song(with a space) \ntype l to see a list of available songs \ntype q to quit the jukebox:\n");
    fgets(input, sizeof(input), stdin);
    //printf("did dat fgets\n");
    parse(input, socket_id);
  }
  return 0;
}
