#ifndef __GF2D_UI_HELPER_FUNCTIONS_
#define __GF2D_UI_HELPER_FUNCTIONS_
#include "simple_json.h"
#include "simple_json_array.h"
#include <string.h>

char * gf2d_ui_helper_functions_get_object_value_as_string(SJson *object,char * key);
char * gf2d_ui_helper_functions_remove_quotes(char * word);
char * gf2d_ui_helper_functions_json_file_location(char * file_location);



#endif
