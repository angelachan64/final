#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef LINUX

int playsong(char * song){
  char * pointers[3];
  
  #ifdef LINUX
  pointers[0] = "aplay";
  #endif
  #else
  pointers[0] = "afplay";
  pointers[1] = song;
  pointers[2] = NULL;
  execvp(pointers[0], pointers);
  return 0;
}

int main(){
  playsong("State_of_Grace.mp3");
  return 0;
}