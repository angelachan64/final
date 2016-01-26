#ifndef MAP

#include <ncurses.h>
#include <string.h>

#define MAP
#define MAP_MEMORY_SIZE 4096
#define MAP_WIDTH 64
#define MAP_HEIGHT 20

void init(char* map);
void display_printf(char* map);
void display_printw(char* map);
void update(char* map);

#endif
