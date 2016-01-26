#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "jukebox.h"
#include <sys/uio.h>
#include <sys/stat.h>
#include <unistd.h>




void handle_client(int socket_client){
  char buf[64];

  while (  read(socket_client, buf, sizeof(buf))){
  	// printf("analyzing input: %s\n", buf);
    char substr[2];
    memcpy (substr, &buf[0], 1);
    substr[1] = '\n';
    if (buf[0] == 'l'){
    	// printf("Recieved 'l', listing songs\n");
   		list_songs(socket_client);
    }
    else if (buf[0] == 'q'){
		// printf("Recieved 'q', quitting\n");   
    	exit(0);
    }
    else if (buf[0] == 'p'){
	    // printf("Recieved 'p', playing a song\n");
	    send_song(buf, socket_client);
    }
    else{
    	// printf("Sorry, could not comprehend that command\n");
    }
  }

}

int send_song(char * user_input, int socket_client){
    char substr[4];
    memcpy(substr, &user_input[2], 3); //get 3 dig song num
    substr[3] = '\0';
    char song_title[64];
    DIR * music_dir = opendir("music");
    struct dirent *file;
    int i = 0;
    char * test_title;
    // printf("boutta find the right file[%s]\n", substr);
    while((file = readdir(music_dir))){
    	if (i > 1){ 
			test_title = file->d_name;
			char sub_title[4];
			memcpy(sub_title, test_title, 3);
			sub_title[3] = '\0';
			// printf("testing title %s with sub %s\n", test_title, sub_title);
			if (strcmp(sub_title, substr) == 0){
				// printf("found the match %d %s\n", i, sub_title);
				// printf("test_title : %s \n", test_title);
			  	strcpy(song_title, test_title);
				//^ buffer overflow possible here
			  	song_title[63] = '\0';
			  	break;
			}
    	}
      i ++;
    }
    if (strlen(song_title) == 0){
    	// printf("unable to find song\n");
    	write(socket_client, "-1", 3);
    	return -1;
    }

    // printf("alreadt found the right file [%s]\n", song_title);
    closedir(music_dir);
    char file_path[64] = "music/";
    strncat(file_path, song_title, sizeof(file_path) - 7);
    int song_fd = open(file_path, O_RDONLY);
    // printf("foudn the song_fd : %s \n", file_path);
    //open song file
    if (song_fd < 0){
    	// printf("unable to find the file\n");
    	// printf("errno %s\n", strerror(errno));
    	write(socket_client, "-1", 3);
    	return -1;
    }
    struct stat st; //get file size
    if (stat(file_path, &st) < 0){
        // printf("unable to stat the file\n");
    	// printf("errno %s\n", strerror(errno));
    	write(socket_client, "-1", 3);
    	return -1;
    }
    //read file and correct bytes
    int file_size = st.st_size;
    void * song = calloc(1, file_size); //max ten megabytes
    // printf("file size: %d\n", file_size);
    if (read(song_fd, song, file_size) < 0){
    	// printf("couldn't read into song\n");
    	// printf("errno %s\n", strerror(errno));
    	write(socket_client, "-1", 3);
    	return -1;
    }

    //first send file size
    int tmp = htonl(file_size);
    write(socket_client, &tmp, sizeof(tmp));


    // printf("read in %s\n", (char*) song);
    //send song over socket
    // printf("boutta write song to client\n");
    int total_writ = 0;
    while(total_writ < file_size){
    	// printf("writin writing writin\n");
    	int writ_now = write(socket_client, &song[total_writ], file_size - total_writ);
    	// printf("sent over %s\n", (char*) song);
    	if (writ_now < 0){
    		// printf("couldn't write song to client\n");
    		// printf("errno %s\n", strerror(errno));
    		write(socket_client, "-1", 3);
    		return -1;
    	}
    	total_writ += writ_now;
    }
    // #ifdef __linux__
    // if( sendfile( socket_client, song_fd, 0, file_size) < 0){
     	printf("unable to send? \n");
     	printf("errno %s\n", strerror(errno));
    //  	write(socket_client, "-1", 3);
    //  	return -1;
    //  }
    //  #else
    //  if( sendfile(song_fd, socket_client, 0, 0, 0, 0) < 0){
       printf("unable to send? \n");
       printf("errno %s\n", strerror(errno));
    //    write(socket_client, "-1", 3);
    //    return -1;
    //  }
    //  #endif
    // write(socket_client, song, file_size + 1);
    free(song);
    // printf("finished writing song\n");
    return 0;
}

int list_songs(int socket_client){
  DIR * music_dir = opendir("music");
  struct dirent *file;
  char song_list[1024] = "";
  int i = 0;
  while((file = readdir(music_dir)) && strlen(song_list) < 1024){
  	if (!(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0
  				|| strcmp(file->d_name, "README.md") == 0  ))
  	{   //above condition exludes ., .. and readme entries
      strcat(song_list, file->d_name);
      strcat(song_list, "\n");    
  	}
  }
  closedir(music_dir);
  write(socket_client, song_list, sizeof(song_list));
  // printf("total song list: \n[%s]\n ", song_list);
  return 0;
}

int main() {
  int socket_id, socket_client;
  socket_id = socket( AF_INET, SOCK_STREAM, 0 );
  struct sockaddr_in listener;
  listener.sin_family = AF_INET; 
  listener.sin_port = htons(24601); 
  listener.sin_addr.s_addr = INADDR_ANY; 
  if (bind(socket_id, (struct sockaddr *)&listener, sizeof(listener)) < 0){
  	// printf("unable to bind\n");
  	return -1;
  }
  if (listen( socket_id, 1 ) < 0){
  	// printf("unale to listen\n");
  	return -1;
  }

   printf("The jukebox server is up and running and waiting for clients to connect.\n");
   while (1) {
    socket_client = accept( socket_id, NULL, NULL );
    int cpid = fork();
    if (cpid == 0){
      printf("Handling Client %d\n", socket_client);
      handle_client(socket_client);
    }
   }
  return 0;
}
