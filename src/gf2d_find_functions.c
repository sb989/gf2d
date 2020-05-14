#include "gf2d_find_functions.h"

static List * functions = NULL;


void gf2d_find_functions_init()
{
  functions = gfc_list_new();
  atexit(gf2d_find_functions_clear);
}

Function * gf2d_find_functions_new_function()
{
  Function * temp;
  temp = (Function*) malloc(sizeof(Function));
  temp->name = NULL;
  functions = gfc_list_append(functions,temp);
  return temp;
}

void gf2d_find_functions_add_func(char * name, void (*func)(void*))
{
  Function * temp;

  temp = gf2d_find_functions_new_function();
  temp->name = name;
  temp->func = func;
}

Function * gf2d_find_functions_find_func(char * name)
{
    int i,count;
    Function * temp;
    if(!name)
      return NULL;
    count = gfc_list_get_count(functions);
    for(i = 0;i<count;i++)
    {
        temp = gfc_list_get_nth(functions,i);
        if(strcmp(temp->name,name)==0)
        {
          //slog("found func %s",name);
          return temp;
        }
    }
    //slog("returning null when finding function");
    return NULL;
}

void gf2d_find_functions_clear()
{
  int count,i;
  Function * f;
  count = gfc_list_get_count(functions);
  for(i=0;i<count;i++)
  {
    f = gfc_list_get_nth(functions,i);
    if(f)
      free(f);
  }
  free(functions);

}
