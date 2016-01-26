#include "lib.h"
#define MAXLEN 256
//#define PATH_MAX 2048

void create_new(){
  int f;
  f = fork();
  if(f==-1){
    perror("fork failed");
  }else if(f==0){
    int r;
    r = execlp("touch","touch","new_email.txt",NULL);
    if(r==-1){
      perror("this is what's wrong");
    }
    exit(0);
  }else{
    int exit;
    int pid = wait(&exit);
  }
}

void send_email(char* file_name, int socket_id, int fd){
  printf("entered send_email\n");
  char buffer[2048];
  char final[2048];
  final[0] = 'S';
  final[1] = 'E';
  final[2] = 'N';
  final[3] = 'D';
  final[4] = '\n';

  struct stat fileinfo;
  int f = fstat(fd, &fileinfo);
  //printf("This is f: %d\n",f);
  int filesize = fileinfo.st_size;
  // printf("filesize: %d\n",filesize);

  struct stat fileinfo2;
  int g = stat(file_name, &fileinfo);
  // printf("This is g: %d\n",g);
  int filesize2 = fileinfo.st_size;
  // printf("filesize2: %d\n",filesize2);

  // printf("fd: %d\n",fd);
  fd = open(file_name,O_RDWR,0664);
  // printf("fd: %d\n",fd);

  int off;
  off = lseek(fd,0,SEEK_SET);
  // printf("this is off: %d\n",off);
  if(off==-1){
    perror("lseek failed");
  }
  int r = read(fd,buffer,filesize2);
  if(r==-1){
    perror("Failed to read from file");
  }
  // printf("This is r: %d\n",r);
  // printf("This is buffer: [%s]\n",buffer);
  strncat(final,buffer,strlen(buffer)+1);
  sock_write(socket_id,final);
  close(fd);
  printf("this is what you sent to the server:\n[\n%s\n]\n",final);
}

void open_file(char* buffer, char* final, int socket_id, int fd){
  int f;
  f = fork();
  if(f==-1){
    perror("Failed to fork");
  }else if(f==0){
     int i;
     //i = execlp("xdg-open","xdg-open",final,NULL);
     i = execlp("open","open",final,NULL);
     if(i==-1){
       perror("Failed to open file");
     }
  }else{
    printf("Start writing your email:\n");
    printf("When you are done, press any key to send\n");
    fgets(buffer,MAXLEN,stdin);
    send_email(final, socket_id, fd);
  }
}

void compose(int socket_id){
  printf("Composing...\n");
  change_location("Drafts.d");
  create_new();
  int fd;
  char buffer[256];
  char final[256];
  int done = 0;

  printf("Type your subject line:\n");
  fgets(buffer,MAXLEN,stdin);
  strip_add(buffer,final);
  int len = strlen(final);
  final[len] = '.';
  final[len+1] = 't';
  final[len+2] = 'x';
  final[len+3] = 't';
  final[len+4] = '\0';
  printf("final: [%s]\n",final);
  
  fd = open(final,O_CREAT|O_RDWR,0644);
  printf("This is fd: %d\n",fd);
  if(fd==-1){
    perror("Failed to create file");
  }
  //close(fd);
  printf("This is final now: [%s]\n",final);
  
  open_file(buffer,final,socket_id,fd);

}

void strip_add(char* source, char* dest){
  char* leftovers;
  int overall_len = strlen(source);
  
  leftovers = strchr(source,'\n');
  int leftovers_len = strlen(leftovers);
  int new_len = overall_len - leftovers_len;

  strncat(dest,source,new_len);
}

void change_location(char* location){
  char buffer[256];
  char current_path[256];
  char path[256];
  char* curr = getcwd(buffer,256);

  strcat(current_path,curr);
  printf("current_path: [%s]\n",current_path);
  int overall_len = strlen(current_path);
  char* leftovers = strrchr(current_path,'/');

  if(strcmp(leftovers,"/my_mail.d")!=0){
    printf("leftovers: [%s]\n",leftovers);
    int leftovers_len = strlen(leftovers);
    int new_len = overall_len - leftovers_len;
    strncat(path,current_path,new_len);
  }else{
    strcat(path,current_path);
  }
  printf("path: [%s]\n",path);
   char* sep = "/";
   strcat(path,sep);
   strcat(path,location);
   int r;
   r = chdir(path);
   int i;
   for(i=0;i<20;i++){
     printf("\n");
   }
   printf("Location: [%s]\n",path);
   my_ls();
   for(i=0;i<20;i++){
     printf("\n");
   }
}

void execute(char* cmd, int socket_id){
  printf("you typed this: [%s]\n",cmd);
  if(strncmp(cmd,"go_to",5)==0){
    char* location;
    strsep(&cmd," ");
    location = cmd;
    change_location(location);
    take_directions(socket_id);
  }else if(strncmp(cmd,"compose",7)==0){
    printf("You typed 'compose'\n");
    compose(socket_id);
    take_directions(socket_id);
  }else{
    printf("Not a valid command\n");
  }
}

void take_directions(int socket_id){
  char buffer[256];
  char final[256];
  int   ch;
  char  buf[256];

  printf("Flushing input. Press any key to continue:\n");
  
  while ((ch = getchar()) != '\n' && ch != EOF);
  
  printf ("Enter your command: ");
  int i;
  for(i=0; i<256; i++){
    final[i] = '\0';
  }
  
  if (fgets(buffer, sizeof(buffer), stdin))
  {
    //printf ("You entered: %s", buf);
    strip_add(buffer,final);
    //printf("final: [%s]\n",final);
    execute(final, socket_id);
  }
}

void my_ls(){
  int f;
  f = fork();
  if(f==-1){
    perror("fork failed");
  }else if(f==0){
    int r;
    r = execl("/bin/ls","ls",NULL);
    if(r==-1){
      perror("this is what's wrong");
    }
    exit(0);
  }else{
    printf("Contents:\n");
    int exit;
    int pid = wait(&exit);
  }
  printf("\nCommands:\n");
  printf("Type 'go_to' and the folder name to enter that folder\n");
  printf("(other commands as we create them)");
}

void enter_mail(int socket_id){
  int i;
  for(i=0;i<10;i++){
    printf("\n");
  }
    printf("Welcome to my_mail!\n");
    char buffer[256];
    char* path = getcwd(buffer,256);
    char* new_path = "/my_mail.d";
    strcat(path,new_path);
    printf("Location: [%s]\n",path);
    int r;
    r = chdir(path);
    my_ls();
    for(i=0;i<10;i++){
      printf("\n");
    } 
    take_directions(socket_id);
}

void sign_in(int socket_id){
  char use[256];
  char pass[256];
  char final[256];
  char* use_phrase = "LOGIN\nUsername: ";
  char* pass_phrase = "\nPassword: ";

  strcat(final,use_phrase);
  printf("Enter your username: ");
  fgets(use,MAXLEN,stdin);
  strip_add(use,final);

  strcat(final,pass_phrase);
  printf("Enter your password: ");
  fgets(pass,MAXLEN,stdin);
  strip_add(pass,final);

  sock_write(socket_id,final);
  printf("This is what you sent to the server:\n[%s]\n",final);
  
  //read from socket
  char buffer[256];
  int r = read(socket_id, buffer, 256);
  printf("r: %d\n",r);
  if(r==-1){
    perror("Failed to read from socket");
  }
  printf("contents of buffer: [%s]\n",buffer);
  if(strncmp(buffer,"OK",2)==0){
    enter_mail(socket_id);
  }else if(strncmp(buffer,"FAIL\nIncorrect password",23)==0){
    printf("Incorrect password\n");
  }else if(strncmp(buffer,"FAIL\nNo such user",17)==0){
    printf("No such user\n");
  }else{
    printf("Unspecified error\n");
  }
}

void choose_username(char* final, char* use){
  printf("Enter your new username: ");
  fgets(use,MAXLEN,stdin);
  strip_add(use,final);
}

void choose_password(char* final, char* pass, char* pass2, int socket_id){
  printf("Enter your new password: ");
  fgets(pass,MAXLEN,stdin);
  printf("Re-enter your new password: ");
  fgets(pass2,MAXLEN,stdin);

  if(strcmp(pass,pass2)==0){
    strip_add(pass,final);
    sock_write(socket_id,final);
  }else{
    printf("Passwords did not match. Try again:\n");
    choose_password(final, pass, pass2, socket_id);
  }
}

void sign_up(int socket_id){
  char use[256];
  char pass[256];
  char pass2[256];
  char final[256];
  char* use_phrase = "SETUP\nUsername: ";
  char* pass_phrase = "\nPassword: ";

   int i;
  i = fork();
  if(i==-1){
    printf("Error\n");
  }else if(i==0){

  }else{
    strcat(final,use_phrase);
    choose_username(final, use);
    strcat(final,pass_phrase);
    choose_password(final, pass, pass2, socket_id);

    printf("This is what you sent to the server:\n[%s]\n",final);
  }
}

void check_for_account(char *buffer, int socket_id){
  if(strncmp(buffer,"y\n",2)==0){
    sign_in(socket_id);
  }else if(strncmp(buffer,"n\n",2)==0){
    sign_up(socket_id);
  }else{
    printf("Not a valid response. Enter another response: ");
    fgets(buffer,sizeof(stdin)-1,stdin);
    check_for_account(buffer, socket_id);
  }
}



int main() {

  // Step 1. Create the socket (same as server code)
  int socket_id = socket(AF_INET, SOCK_STREAM, 0);

  // Step 2. Bind to port/address
  struct sockaddr_in sock;
  //memset(&sock, '\0', sizeof(sock));
  sock.sin_family = AF_INET;
  sock.sin_port = htons(5000);
  // void: parses a string into a binary IP address
  inet_aton( "127.0.0.1", &(sock.sin_addr) );
  // The real action
  bind(socket_id, (struct sockaddr *) &sock, sizeof(sock));

  // Step 3. Attempt to create a connection
  // i is a message

  int i = connect(socket_id, (struct sockaddr *) &sock, sizeof(sock));
  printf("<client> connect returned: %d\n", i);
  if(i==-1){
    perror("");
  }

  // Step 4. Do network stuff
  char buffer[256];
  //read(socket_id, buffer, sizeof(buffer) - 1);
  
  //Request info from user
  printf("Do you already have an account? (y/n)\n");
  //read info
  fgets(buffer,sizeof(stdin)-1,stdin);
  check_for_account(buffer, socket_id);
  
  return 0;

  
}
