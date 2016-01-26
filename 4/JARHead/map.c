#include "map.h"

void init(char* map) {
  memset(map, 0, sizeof(map));
  int i;
  for (i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i) {
    strcat(map, " ");
  }
}

void display_printf(char* map) {
  int i;
  for (i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i) {
    if (i % MAP_WIDTH == 0 && i) {
      printf("\n");
    }
    printf("%c", map[i]);
  }
  printf("\n");
}

void display_printw(char* map) {
  int i;
  for (i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i) {
    if (i % MAP_WIDTH == 0 && i) {
      printw("\n");
    }
    printw("%c", map[i]);
  }
  printw("\n");
}

void update(char* map) {
}
