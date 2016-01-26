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

int newuser() {
  int moveon = 0;
  char passwd[16];
  int username = 0;
  while (moveon == 0) {
    printf("Type in your 4-digit ID for the username.\n");
    char buffer[100];
    fgets(buffer, 100, stdin);
    if (sscanf(buffer, "%d", &username)) {
      if (username >= 1000 && username <= 9999) {
	char *in = passwd;
	struct termios  tty_orig;
	char c;
	tcgetattr( STDIN_FILENO, &tty_orig );
	struct termios  tty_work = tty_orig;
	puts("Please input password:");
	tty_work.c_lflag &= ~( ECHO | ICANON );
	tty_work.c_cc[ VMIN ]  = 1;
	tty_work.c_cc[ VTIME ] = 0;
	tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_work );
	while (1) {
	  if (read(STDIN_FILENO, &c, sizeof c) > 0) {
	    if ('\n' == c) {
	      break;
	    }
	    *in++ = c;
	    write(STDOUT_FILENO, "*", 1);
	  }
	}
	tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_orig );

	*in = '\0';
	fputc('\n', stdout);

	char check[16];
	char *checkin =  check;
	struct termios  tty_check;
	char d;
	tcgetattr( STDIN_FILENO, &tty_check );
	struct termios  tty_checking = tty_check;
	puts("Please retype password:");
	tty_checking.c_lflag &= ~( ECHO | ICANON );
	tty_checking.c_cc[ VMIN ]  = 1;
	tty_checking.c_cc[ VTIME ] = 0;
	tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_checking );
	while (1) {
	  if (read(STDIN_FILENO, &d, sizeof(d)) > 0) {
	    if ('\n' == d) {
	      break;
	    }
	    *checkin++ = d;
	    write(STDOUT_FILENO, "*", 1);
	  }
	}

	tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_check );

	*checkin = '\0';
	fputc('\n', stdout);

	if(strcmp(passwd,check) != 0) {
	  printf("Error: Passwords don't match. Please try again.\n");
	} else {
	  char newuser[1000];
	  snprintf(newuser,sizeof(newuser), "%i: %s\n",username,passwd);
	  int fd = open("tutoraccounts.txt",O_RDWR | O_APPEND,0644);
	  write(fd, newuser, strlen(newuser));
	  close(fd);
	  moveon = 1;
	}
      } else {
	printf("Error: Invalid username. Please input your 4-digit ID.\n");
      }
    }
  }
  return username;
}

int does_user_exist(char* username) {
  FILE *fp;
  int line_num = 1;
  int find_result = 0;
  char temp[512];

  if((fp = fopen("tutoraccounts.txt", "r")) == NULL) {
    return(-1);
  }
  while(fgets(temp, 512, fp) != NULL) {
    if((strstr(temp, username)) != NULL) {
      printf("A match found on line: %d\n", line_num);
      printf("\n%s\n", temp);
      find_result++;
    }
    line_num++;
  }
  return find_result;
}

int registereduser() {
  FILE* fd = fopen("tutoraccounts.txt","r");
  printf("Type in your 4-digit ID for the username.\n");
  int username;
  scanf("%i",&username);
  char passwd[16];
  if (1000 <= username && username <= 9999) {
    /*   
 char buffer[100];
    strcpy(buffer,username);
    int num;
    num = does_user_exist(buffer);
    printf("%s\n",buffer);
    //     if (does_user_exist(buffer) != 0) {
    */    
char *in = passwd;
    struct termios  tty_orig;
    char c;
    tcgetattr( STDIN_FILENO, &tty_orig );
    struct termios  tty_work = tty_orig;
    puts("Please input password:");
    tty_work.c_lflag &= ~( ECHO | ICANON );
    tty_work.c_cc[ VMIN ]  = 1;
    tty_work.c_cc[ VTIME ] = 0;
    tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_work );

    while (1) {
      if (read(STDIN_FILENO, &c, sizeof c) > 0) {
	if ('\n' == c) {
	  break;
	}
	*in++ = c;
	write(STDOUT_FILENO, "*", 1);
      }
    }

    tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_orig );

    *in = '\0';
    fputc('\n', stdout);
    return username;
    //  }
  } else {
    printf("That account doesn't exist yet. Please register for a new account.\n");
    username = newuser();
    return username;
  }

}

int tutorlogin() {
  int tutoraccounts = open("tutoraccounts.txt", O_CREAT | O_RDWR | O_APPEND, 0644);
  if (tutoraccounts < 0) {
    printf("Error %d: %s\n", errno, strerror(errno));
  }

  int accessing = 0;
  char str[100];
  int username = 0;
  while (accessing == 0) {
    printf("Press 1 to login or 2 to register.\n");
    int action;
    char buffer[100];
    fgets(buffer, 100, stdin);
    if (sscanf(buffer, "%d", &action)) {
      if (action == 1) {
	username = registereduser();
	accessing = 1;
      } else if (action == 2) {
	username = newuser();
	accessing = 1;
      } 
    } else {
      printf("We didn't understand your response. Please try again.\n");
    }
  }

  //search for username in tutoraccounts.txt
  //if it exists, ask for password
  //if not, ask to create account and double check password
  
  printf("%i\n",username);
  return username;
}

int clean_stdin() {
  while (getchar() != '\n');
  return 1;
}


int main() {
  int tutoraccounts = open("tutoraccounts.txt", O_CREAT | O_TRUNC, 0644);
  if (tutoraccounts < 0) {
    printf("Error %d: %s\n", errno, strerror(errno));
  } else {
    printf("Program loaded.\n");
  }
  close(tutoraccounts);
  printf("Hello and welcome to ARISTA chat!\n");
  int moveon = 0;
  while (moveon == 0) {
    printf("\nType 1 if you are a tutor or 2 if you are a tutee.\n");
    int student;
    char buffer[100];
    fgets(buffer, 100, stdin);
    if (sscanf(buffer, "%d", &student)) {
      if (student == 1) {
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
      } else if (student == 2) {
	printf("\nWelcome tutee!\n");
	moveon =1;
      } 
    } else {
      printf("We didn't understand your response. Please try again.\n");
    }
  }
}

