#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int playsong(){

//char * command1[3];
//char * command2[3];
   // int pid = fork();
   // if (pid == 0){
  char * pointers[3];
  
  #ifdef __linux__
  pointers[0] = "mpg321";
  #else
  pointers[0] = "afplay";
  #endif
  pointers[1] = "temp.mp3";
  pointers[2] = NULL;
  execvp(pointers[0], pointers);
// }
  return 0;
}
