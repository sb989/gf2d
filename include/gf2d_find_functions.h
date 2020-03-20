#ifndef __GF2D_FIND_FUNCTIONS
#define __GF2D_FIND_FUNCTIONS
#include "gfc_list.h"
#include "simple_logger.h"
#include <string.h>
typedef struct
{
  char * name;
  void (*func)(void*);
}Function;

void gf2d_find_functions_init();
Function *gf2d_find_functions_new_function();
void gf2d_find_functions_add_func(char * name, void (*func)(void*));
Function *gf2d_find_functions_find_func(char * name);
void gf2d_find_functions_clear();



#endif
