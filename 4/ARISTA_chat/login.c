#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <termios.h>
#include <unistd.h>

int getch() {
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}


int newuser() {
  printf("Type in your 4-digit ID for the username.\n");
  int username = 0;
  scanf("%i",&username);
/*
  printf("\n Type in a  password: ");
  char pwd[25],ch='a';
  int i=0;
  while(1) {
    ch=getch();
    if(ch==13) {
      break;
    } else if(ch==8) {
      if(i!=0) { //this is for avoiding the ENTER instructions getting deleted 
	printf("\b");  //printing backspace to move cursor 1 pos back
	printf("%c",32);//making the char invisible which is already on console
	printf("\b"); //printing backspace to move cursor 1 pos back
	i--;
	pwd[i]='\0';
      } else {
	continue;
      }
    } else {
      putchar('*');/* char - '*' will be printed instead of the character 
      pwd[i]=ch;
      i++;
    }
  }
*/
  return username;
}

int registereduser() {

	return 1;

}

int tutorlogin() {
  int tutoraccounts = open("tutoraccounts.txt", O_CREAT | O_RDWR | O_APPEND, 0644);
  if (tutoraccounts < 0) {
    printf("Error %d: %s\n", errno, strerror(errno));
  }

  int accessing = 0;
  int username = 0;
  while (accessing == 0) {
	printf("Press 1 to login or 2 to register.\n");
	int action;
	scanf("%i",&action);
	if (action == 1) {
      		username = registereduser();
      		break;
   	 } else if (action == 2) {
        	username = newuser();
      		break;
 	} else {
      		printf("We didn't understand your response. Please try again.\n");
    		continue;
	}
  }

  //search for username in tutoraccounts.txt
  //if it exists, ask for password
  //if not, ask to create account and double check password

	printf("%i\n",username);
  return username;
}

int main() {
  int tutoraccounts = open("tutoraccounts.txt", O_CREAT | O_TRUNC, 0644);
  if (tutoraccounts < 0) {
    printf("Error %d: %s\n", errno, strerror(errno));
  } else {
    printf("Program loaded.\n");
  }
  close(tutoraccounts);

  printf("\nHello and welcome to MovieFone--wait, sorry. Wrong program\n");
  printf("Hello and welcome to ARISTA chat!\n");

  int moveon = 0;
  while (moveon == 0) {
    printf("\nAre you a tutor or a tutee?\n");
    char student[100];
    fgets(student, sizeof(student), stdin);

    if (strcmp(student,"tutor") >= 0) {
      printf("\nWelcome tutor! Please login.\n");
      int loggedin = 0;
      while (loggedin == 0)
	if (tutorlogin() != 0) {
	  printf("Thank you for logging in\n");
	  moveon = 1;
	  loggedin = 1;
	} else {
	  printf("There was an error while logging in.");
	  printf("Please try again.\n");
	}
    } else if (strcmp(student, "tutee") >= 0) {
      printf("\nWelcome tutee!\n");
      moveon =1;
    } else {
      printf("We didn't understand your response. Please try again.\n");
    }
  }
}
