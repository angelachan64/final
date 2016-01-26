#ifndef _BOMB_HELPER_H_
#define _BOMB_HELPER_H_

typedef struct module {
  int complete;
  int steps;
  int current_step;
  char** visual_representation;
} module;

module *create_module(char* filename);

#endif 
