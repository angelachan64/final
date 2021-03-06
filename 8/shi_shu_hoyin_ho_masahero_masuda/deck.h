#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct card{
  char* content;
  char* type;
  int owner;
}card;

card* makecard(char* content,char* type){
  card* out = (card*)malloc(sizeof(card));
  out->type = type;
  out->content = content;
  return out;
}

card deal_greencard(card *green_deck);
card deal_redcard(card *red_deck);
void printdeck(card* deck){
  int i = 0;
  while(deck[i].content){
    printf("%s\n",deck[i].content);
    i++;
  }
}
int getsize(card* deck){
  int counter = 0;
  while(deck[counter].content){
    counter++;
  }
  return counter;
}

unsigned int randNum(){
  int descriptor = open("/dev/urandom", O_RDONLY);
  unsigned int *num = (unsigned int *)malloc(4);
  read(descriptor,num,4);
  close(descriptor);
  return *num;
}

void shuffle(card* deck){
  int len = getsize(deck);
  int rand1;
  int rand2;
  //  card* temp = (card*)malloc(sizeof(card));
  card *temp;
  int counter = 0;
  while(counter < 1000){
    rand1 = randNum() % len;
    rand2 = randNum() % len;
    temp = &deck[rand1];
    deck[rand1] = deck[rand2];
    deck[rand2] = *temp;
    counter ++;
  }
}

struct card* makedeck(char* type){
  card* deck;
  int descriptor;
  char buffer[20000];
  if(type == "red"){
    descriptor = open("reddeck",O_RDONLY);
    deck = (card*)malloc(sizeof(card)*746);
  }
  if(type == "green"){
    descriptor = open("greendeck",O_RDONLY);
    deck = (card*)malloc(sizeof(card)*250);
  }
  read(descriptor,buffer,sizeof(buffer));
  char* cards = buffer;
  char* temp;
  int i = 0;
  while(cards){
    temp = strsep(&cards,"\n");
    deck[i] = *makecard(temp,type);
    i ++;
  }
  return deck;
}


