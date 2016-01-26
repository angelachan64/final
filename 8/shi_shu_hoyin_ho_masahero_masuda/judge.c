#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "deck.h"

int judge_handshake(int* from_server){
  int to_server;
  char buffer[100];

  sprintf(buffer,"%d",getpid());
  mkfifo(buffer,0644);

  to_server = open("pipe",O_WRONLY);
  write(to_server,buffer,sizeof(buffer));

  *from_server = open(buffer,O_RDONLY);
  remove(buffer);

  read(*from_server,buffer,sizeof(buffer));
  printf("Judge connected: %s\n",buffer);

  return to_server;
  
}

card* receivecards(int* from_server,card* received){
  card buffer[100];
  int counter = 0;
  card *tobejudged = (card* )malloc(sizeof(card)*8);
  while(counter < 8){
    read(*from_server,buffer,sizeof(buffer));
    tobejudged[counter] = *makecard(buffer,"red");
    counter ++;
  }
  return tobejudged;
}

int main(){
  
  card received[8];
  int to_server;
  int from_server;
  char buffer[100];
  int winner;
  
  to_server = judge_handshake(&from_server);
  while(1){
    printf("select a card as the winner: \n");
    int counter = 0;
    while(counter < 8 && received[counter].content){
      printf("%d.%s\n",counter,received[counter].content);
    }
    fgets(buffer,sizeof(buffer),stdin);
    *strchr(buffer,"\n") = 0;
    int winner = atoi(buffer);
    while(winner < 0 || winner > 7 || !(received[winner].content)){
      printf("please type in a valid number: \n");
      while(counter < 8 && received[counter].content){
	printf("%d.%s\n",counter,received[counter].content);
      }
      fgets(buffer,sizeof(buffer),stdin);
      *strchr(buffer,"\n") = 0;
      int winner = atoi(buffer);
    }
    write(to_server,&card[winner].content,sizeof(card[winner].content));
    
  }
  
  close(to_server);
  close(from_server);
  return 0;
}
