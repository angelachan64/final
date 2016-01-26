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
/**
 * Takes an index in a MAP_WIDTH * MAP_HEIGHT size map and returns the
 * corresponding x coordinate.
 */
int index_to_x(int index);
/**
 * Takes an index in a MAP_WIDTH * MAP_HEIGHT size map and returns the
 * corresponding y coordinate.
 */
int index_to_y(int index);

void add_new_player(char* map, char player);
/**
 * char* map: the map, memory size: MAP_WIDTH * MAP_HEIGHT, essentially this is
 * a grid but its stored 1 dimensionally instead of 2d. To get the rows and
 * columns just do the necessary math.
 * 
 * char player: the player's character
 * 
 * int up: the amount to move the player up, can be positive or negative
 * if this will move the player out of the map, then nothing will happen.
 * 
 * int left: the amount to move the player left, can be positive or negative
 * if this will move the player out of the map, then nothing will happen.
 */
void move_player(char* map, char player, int up, int left);

#endif
