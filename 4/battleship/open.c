#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

int main(){
  
  int i,n,len_of_input;
  time_t t;

  srand((unsigned) time(&t));
  n = rand() % 3;
  
  int p,p1;
  p = fork();
  if (p != 0) {
    int status = 1;
    //wait(&status);
  }
  else {
    execlp("afplay", "afplay", "song.mp3", 0);
  }
  
    p1 = fork();
    if (p1 != 0) {
      int status1 = 1;
      wait(&status1);
    }
    else{
      sleep(2.5);
      printf("OOOOOO\n");
      printf("OO   OO\n");
      printf("OO  OO\n");
      printf("OOOOOO\n");
      printf("OO   OO\n");
      printf("OOOOOO\n\n");
      sleep(1);
      printf("       OOOO\n");
      printf("      OO  OO\n");
      printf("      OO  OO\n");
      printf("      OOOOOO\n");
      printf("      OO  OO\n");
      printf("      OO  OO\n\n");
      sleep(1);
      printf("            OOOOOO\n");
      printf("              OO  \n");
      printf("              OO\n");
      printf("              OO\n");
      printf("              OO\n");
      printf("              OO\n\n");
      sleep(1);
      printf("      OOOOOO\n");
      printf("        OO  \n");
      printf("        OO\n");
      printf("        OO\n");
      printf("        OO\n");
      printf("        OO\n\n");
      sleep(1);
      printf("OO\n");
      printf("OO\n");
      printf("OO\n");
      printf("OO\n");
      printf("OO\n");
      printf("OOOOOO\n\n");
      sleep(1);
      printf("      OOOOOO\n");
      printf("      OO\n");
      printf("      OO\n");
      printf("      OOOO\n");
      printf("      OO\n");
      printf("      OOOOOO\n\n");
      sleep(1);
      printf("           OOOOOO\n");
      printf("           OO\n");
      printf("           OO\n");
      printf("           OOOOOO\n");
      printf("              OOO\n");
      printf("          OOOOOO\n\n");
      sleep(1);
      printf("      OO  OO\n");
      printf("      OO  OO\n");
      printf("      OOOOOO\n");
      printf("      OOOOOO\n");
      printf("      OO  OO\n");
      printf("      OO  OO\n\n");
      sleep(1);
      printf("  OO\n");
      printf("  OO\n");
      printf("  OO\n");
      printf("  OO\n");
      printf("  OO\n");
      printf("  OO\n\n");
      sleep(1);
      printf("OOOOO\n");
      printf("OO  O\n");
      printf("OO  O\n");
      printf("OOOO\n");
      printf("OO\n");
      printf("OO\n\n");
      sleep(1);
      printf("                         OOOOOO     OOOO    OOOOOO   OOOOOO   OO       OOOOOO    OOOOOO   OO  OO   OO   OOOOO\n");
      printf("                         OO   OO   OO  OO     OO       OO     OO       OO        OO       OO  OO   OO   OO  O\n");
      printf("                         OO  OO    OO  OO     OO       OO     OO       OO        OO       OOOOOO   OO   OO  O\n");
      printf("                         OOOOOO    OOOOOO     OO       OO     OO       OOOO      OOOOOO   OOOOOO   OO   OOOO\n");
      printf("                         OO   OO   OO  OO     OO       OO     OO       OO           OOO   OO  OO   OO   OO \n");
      printf("                         OOOOOO    OO  OO     OO       OO     OOOOOO   OOOOOO    OOOOOO   OO  OO   OO   OO\n\n");
      
      sleep(1);
      printf("WELCOME PLAYER\n");
      sleep(1);
      printf("ENTER YOUR NAME\n");
      
      char user_input[256];
      fgets(user_input, sizeof(user_input), stdin);
      len_of_input = strlen(user_input);
      user_input[len_of_input-1] = 0;
      
      printf("SO YOUR NAME IS: %s ", user_input);
      if (n == 1){
	printf("\nThat's nice...\n");
      }
      else printf ("\nNice!\n");
      
    } 
    
}
