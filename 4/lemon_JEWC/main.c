#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <sys/time.h>

/*kbhit implementation taken from http://http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
 */

void changemode(int dir)
{
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
 
int kbhit (void)
{
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
 
}


/*Modified from 
http://www.sanfoundry.com/c-program-insert-word-desired-location/
*/
void editstring()
{
    int i, j, count = 0, pos, flag = 0;
    char s1[100], s2[10], s3[100];
    char *ptr1, *ptr2, *ptr3;
 
    printf("\nenter the String:");
    scanf(" %[^\n]s", s1);
    printf("\nenter the string to be inserted:");
    scanf(" %[^\n]s", s2);
    printf("\nenter the position you like to insert:");
    scanf("%d", &pos);
 
    ptr1 = s1;
    ptr3 = s3;
    /*COPYING THE GIVEN STRING TO NEW ARRAY AND INSERTING THE STRING IN NEW ARRAY*/
    for (i = 0, j = 0;*ptr1 != '\0'; ptr1++, i++, j++, ptr3++)
    {
        s3[j] = s1[i];
        if (*ptr1 == ' ' && flag != 1)
            ++count;
        if (flag != 1 && count == pos - 1)
        {
            flag = 1;
            for(ptr2 = s2;*ptr2 != '\0'; ptr2++)
            {
                s3[++j] = *ptr2;
                ptr3++;
            }
            s3[++j] = ' ';
            ptr3++;
        }
    }
    s3[j] = '\0';
    printf("\nthe string after modification is\n\n %s\n", s3);
}
/**
void append(char old[], const char new[], int index) {
    char buf[1000]; 
    strncpy(buf, old, index); 
    int len = strlen(buf);
    strcpy(buf+len, new);
    len += strlen(new);  
    strcpy(buf+len, old+index); 
    strcpy(old, buf);   
}
*/

void append(char old[], char new, int index) {
    char buf[1000]; 
    char new2[1000];
    new2[0]=new;
    strncpy(buf, old, index); 
    int len = strlen(buf);
    strcpy(buf+len, new2);
    len += strlen(new2);  
    strcpy(buf+len, old+index); 
    strcpy(old, buf);   
}


void del_char(char str[],int dex){
  int z=dex;
  while(str[z]){
    //printf("%c\n",str[z]);
    str[z]=str[z+1];
    z++;
  }
}

int main(){
  int file;
  printf("\e[7mPlease enter the name of the file you are creating or editing.\e[27m\n");
  char response[100];
  fgets(response,100,stdin);
  strtok(response,"\n");
  if(access(response,F_OK)!=-1){
    printf("\e[7m%s\e[27m exists. Now opening.\n\n",response);

    int pid1 = fork();
    if(!pid1){ 
    execlp("cat","cat",response,NULL);
    }
  }
  else{
    printf("\e[7m%s\e[27m does not exist. Now creating.\n",response);
    file=open(response, O_CREAT, 0644);
    close(file);
  }
  //Opening file
  file=open(response, O_RDWR, 0644);
  char buffer[1000];
  char buffer2[1000];
  char buffer3[1000];
  char buffer4[1000];
  
  int ch;
  int numread;
  int ch2;

  int cursorx = 0;
  int cursory = 0;

  while(1){
    changemode(1);
    while(!kbhit()){
      //print whole file
      numread = read(file,buffer,1000);
      printf("\e[1;1H\e[2J");
      printf("\e[7mCursor X: %d\nCursor Y: %d\e[27m\n",cursorx,cursory);
      printf("%s",buffer);
      sleep(1);
    }
    ch = getchar();
    //Do stuff when key is pressed
    printf("Entered %c\n",ch);
    printf("Number: %d\n",ch);
    if(ch == 27){
      ch2 = getchar();
      ch2 = getchar();
      printf("Detail: %d\n",ch2);
      if(ch2 == 65){
	if(cursory > 0){
	  cursory--;
	}
      }
      else if(ch2 == 66){
	cursory++;
      }
      else if(ch2 == 68){
	if(cursorx > 0){
	  cursorx--;
	}
      }
      else if(ch2 == 67){
	cursorx++;
      }
      //Up = 65 Down = 66 Right = 67 Left = 68
    }
    changemode(0);
    if(ch != 27){
      //printf("%d",ch);
      //break;
      //cursorx, cursory
      int currentx = 0;
      int currenty = 0;
      int overallid = 0;
      int wedoneyet = 0;
      while(!wedoneyet){
	/*printf("overallid %d\n",overallid);
        if(overallid == 6){
	  printf("6 is %c = %d",buffer[overallid],buffer[overallid]);
	  break;
	  }*/
	if(currentx == cursorx && currenty == cursory){
	  wedoneyet = 1;
	}
	else if(buffer[overallid] == 10){
	  currentx = 0;
	  currenty++;
	  if(currenty > cursory){
	    break;
	  }
	}
	else{
	  currentx++;
	}
	overallid++;
      }
      //after while loop
      overallid--;
      printf("Overall ID of your cursor is %d\n",overallid);
      printf("Which is %c\n\n",buffer[overallid]);
      //this strncpy causes a segmentation fault
      strncpy(buffer2,buffer,overallid+1); //copy first part of buffer to buffer2
      strcat(buffer[overallid],buffer2); //add new char to buffer2
      strcpy(buffer3,buffer+overallid); //copy rest of string to buffer3
      strcat(buffer3,buffer2);
      printf(buffer2);
      
      
      
      
      break;
    }
  }
  
  //closing file
  close(file);
  return 0;
}
 
