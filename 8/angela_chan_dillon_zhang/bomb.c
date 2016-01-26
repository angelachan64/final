#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "color.c"
#include "bomb_helper.h"

int main() {
  textcolor(BRIGHT, RED, BLACK);
  printf("This is the Bomb component.\n");
  textcolor(RESET, WHITE, BLACK);

  create_module("./asciiart/symbols/s1.dat");
  return 0;
}
