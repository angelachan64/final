#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "values.h"
#include "server.h"

//Need to be global for sighandler to work
int socket_id;
int socket_client;
int ppid;

static void sighandler(int signo){
  int status;
  int error;
  if (signo==SIGPIPE){
    error=close(socket_client);
    if (error == -1)
	perror("Error closing client socket\n");
    printf("Client disconnected, child exiting\n");
    exit(42);
  }
  if (signo==SIGINT){
    while (wait(NULL) > 0){
      //parent waits until all children have exited
      ;
    }
    if (getppid() != ppid){
      //Exit procedure for children
      printf("Child exiting\n");
      error=close(socket_client);
      if (error == -1)
	perror("Error closing client socket\n");
      exit(42);
    }
    else{
      error=close(socket_id);
      if (error == -1)
	perror("Error closing main socket\n");
      printf("Main socket closed\n");
      error=semctl(semget(ftok(file_path, sem_id),0,0),0,IPC_RMID,0);
      if (error==-1)
	printf("Error removing semaphores: %s\n",strerror(errno));
      printf("Semaphores removed\n");
      exit(42);
    }
  }
}

void setup(){
  int error=mkdir("root", 0777);
  if (error == -1)
    perror("Root directory already exists\n");
  printf("Root directory established\n");
  int fd = open("root/log.txt", O_RDWR | O_APPEND | O_CREAT,0664);
  if (fd < 0) {
    perror("Error creating log file\n");
    exit(-1);
  }
  error = close(fd);
  if (error == -1){
    perror("Error closing log file\n");
    exit(42);
  }
  printf("log file established \n");
  fd = open("root/users.txt", O_RDWR | O_APPEND | O_CREAT, 0664);
  if (fd < 0) {
    perror("Error creating user file\n");
    exit(-1);
  }
  error = close(fd);
  if (error == -1){
    perror("Error closing user file\n");
 (f(f(f (f  exit(42);
  }
  printf("userlist established\n");
  int semaphore=semget(ftok(file_path, sem_id),2,0664 | IPC_CREAT | IPC_EXCL);
  if (semaphore==-1){
    printf("Error creating log & userlist semaphore: %s\n",strerror(errno));
    exit(42);
  }
  union semun command;
  unsigned short forks[2]={100,100};
  command.array=forks;
  error=semctl(semaphore,0,SETALL,command);
  if (error==-1){
    printf("Error setting semaphore value: %s\n",strerror(errno));
    exit(42);
  }
}

void get_user(char * ans, char name[], char password[], int socket_client){
  int error=read(socket_client,ans,1);
  if (error==-1){
    printf("Error recieving ANS: %s\n",strerror(errno));
    exit(42);
  }
  read(socket_client,name,NAME_LEN);
  if (error==-1){
    printf("Error recieving NAME: %s\n",strerror(errno));
    exit(42);
  }
  read(socket_client,password,PASS_LEN);
  if (error==-1){
    printf("Error recieving PASSWORD: %s\n",strerror(errno));
    exit(42);
  }
}

void send_mail(char name[], int socket_client){
  int error;
  char file_path[NAME_LEN+5]="root/";
  strcat(file_path,name);
  error=access(file_path,F_OK);
  //Check for any messages
  if (error == -1){
    printf("No Messages for %s: %s\n",name,strerror(errno));
    return;
  }
  struct stat buf;
  stat(file_path,&buf);
  int size = buf.st_size;
  if (size == 0)
    return;
  int fd=open(file_path,O_RDONLY);
  if (fd == -1){
    printf("Error opening %s's messages: %s\n",name,strerror(errno));
    return;
  }
  char buf_out[size];
  error=read(fd,buf_out,size);
  if (error == -1){
    printf("Error reading %s's messages: %s\n",name,strerror(errno));
    return;
  }
  error=write(socket_client,&size,4);
  if (error == -1){
    printf("Error sending %s's message size: %s\n",name,strerror(errno));
    return;
  }
  error=write(socket_client,buf_out,size);
  if (error == -1){
    printf("Error sending %s's messages: %s\n",name,strerror(errno));
    return;
  }
  error=close(fd);
  if (error == -1){
    printf("Error closing %s's messages: %s\n",name,strerror(errno));
    return;
  }
  //Get rid of messages
  error=remove(file_path);
  if (error == -1){
    printf("Error removing %s's messages: %s\n",name,strerror(errno));
    return;
  }
}

void recieve_mail(char name[], int socket_client){
  int error,size;
  char target[NAME_LEN];
  error=read(socket_client,&size,4);
  if (error == -1){
    perror("Error getting target size:\n");
    return;
  }
  //check for exit
  if (size == kill_num)
    raise(SIGPIPE);
  error=read(socket_client,target,size);
  if (error == -1){
    perror("Error getting target:\n");
    return;
  }
  error=read(socket_client,&size,4);
  if (error == -1){
    perror("Error getting message size:\n");
    return;
  }
  char buf_in[size];
  error=read(socket_client,buf_in,size);
  if (error == -1){
    perror("Error getting target:\n");
    return;
  }
  printf("Message to %s : %s\n",target,buf_in);
  char file_path[NAME_LEN+5]="root/";
  strcat(file_path,target);
  int fd=open(file_path,O_APPEND | O_CREAT | O_WRONLY, 0644);
  if (fd == -1){
    perror("Error opening file to store message:\n");
    return;
  }
  error=write(fd,name,strlen(name));
  if (error == -1){
    perror("Error writing sender:\n");
    return;
  }
  error=write(fd,": ",2);
  if (error == -1){
    perror("Error writing space:\n");
    return;
  }
  error=write(fd,buf_in,size);
  if (error == -1){
    perror("Error writing message:\n");
    return;
  }
  error=close(fd);
  if (error == -1){
    perror("Error closing message file:\n");
    return;
  }
}

int main(int argc, char *argv[]){
  ppid=getppid();
  signal(SIGINT,sighandler);
  printf("Establishing user file, log file, root directory. Creating semaphores...\n");
  setup();
  printf("Setup complete \n");
  int child_pid;
  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in listener;
  listener.sin_family = AF_INET; //socket type to IPv4
  listener.sin_port = htons(PORTNUM); //port # 
  listener.sin_addr.s_addr = INADDR_ANY; //bind to any incoming address
  bind(socket_id, (struct sockaddr *)&listener, sizeof(listener));
  listen(socket_id,1);
  printf("Socket established\n");
  while(1){
    printf("<Server> waiting for connection\n");
    socket_client = accept(socket_id, NULL, NULL);
    printf("Connection established\n");
    child_pid=fork();
    if (child_pid==0){
      printf("Child process\n");
      char ans;
      char name[NAME_LEN];
      char password[PASS_LEN];
      get_user(&ans,name,password,socket_client);
      printf("ANS: %c NAME: %s PASSWORD: %s\n",ans,name,password);
      if (ans=='1'){
	//Check if user logged in
	//authenticate password. If correct, move on. Otherwise terminate
      }
      while(1==1){
	//do child stuff
	send_mail(name,socket_client);
	recieve_mail(name,socket_client);
      }
      close(socket_client);
      printf("Connection closed\n");
    }
  }
}

int authenticate(char name[], char password[]){
  /* Returns: boolean
  Checks userlist for username and password
  Checks logged to see if user already logged in
  Returns 0 if correct combination not present or the user is already logged in
  */
  FILE *fr;
  int count1 = 0, count2 = 0, check = 0, i, j, flag;
  fr = fopen("root/log.txt", "rt");
  while(fgets(line, 80, fr) != NULL){
    sscanf (line, "%ld", &elapsed_seconds);
  }
  fclose(fr);
  //open and read the file
  //keep file in stringarray 
  //DON'T FORGET TO CLOSE
  for(i=0; i <= count1 - count2; i++){
    for(j=i; j < i + count2; j++){
      flag = 1;
      if(name[j] != fr[j - i])
	{
	  flag = 0;
	  break;
	}
    }
    if(flag == 1)
      return 0;
    else
      return -1;

  }
}

//int add_user(char name[], char password[]){
  /*Returns boolean
  Checks user to see if name is already taken
  Return 1 if name is available and appends name and password to userlist,
  creates directory folders(mailboxes) for the new user.
  Takes into account semaphores (array)
  Returns 0 if name is taken
  */
  
