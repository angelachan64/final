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

int index_to_x(int index) {
  return index % MAP_WIDTH;
}

int index_to_y(int index) {
  return (index - index_to_x(index)) / MAP_HEIGHT;
}

void add_new_player(char* map, char player) {
  int x = rand() % MAP_WIDTH;
  int y = rand() % MAP_HEIGHT;
  
}

void move_player(char* map, char player, int up, int left) {
  int index = strchr(map, player);
  int x, y;
  x = index_to_x(index);
  y = index_to_y(index);
  
  
}
