#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "bomb_helper.h"

module* create_module(char* filename) {
  module *m = malloc(sizeof(module));
  
  int input_status = open(filename, O_RDONLY);
  if (input_status < 0) {
    printf("ERROR: %s",strerror(errno));
    exit(1);
  }

  char* temp = (char *)malloc(sizeof(char)*49152);
  int reader = read(input_status, &temp, sizeof(&temp));
  if (reader < -1) {
    printf("Error: %s\n", strerror(errno));
    exit(1);
  }

  //execlp("echo", "echo", "-e", temp, NULL);
  printf("%s", &temp);
  /*m->complete = 0;

  int counter = 0;

  

  m->steps = atoi("4"); //strsep(&temp, "\n"));
  printf("%s", strsep(&temp,"\n"));
  printf("%d", m->steps);*/
  return m;
}
