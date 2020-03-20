#include "gf2d_ui_helper_functions.h"


char * gf2d_ui_helper_functions_get_object_value_as_string(SJson *object,char * key)
{
  SJString *output;
  SJson *temp_value;
  char * temp;
  temp_value = sj_object_get_value(object,key);
  if(!temp_value)
    return NULL;
  //slog("key is %s",key);
  //sj_echo(temp_value);
  output = temp_value->get_string(temp_value);
  temp = output->text;
  temp = gf2d_ui_helper_functions_remove_quotes(temp);
  sj_string_free(output);
  return temp;
}

char * gf2d_ui_helper_functions_remove_quotes(char * word)
{
  char * temp;
  int i;
  temp = malloc(sizeof(char)*strlen(word));
  for(i = 0;i<strlen(word)-2;i++)
  {
    temp[i] = word[i+1];
  }
  temp[i] = '\0';
  return temp;
}

char * gf2d_ui_helper_functions_json_file_location(char * file_location)
{
  char *temp;
  temp = malloc(sizeof(char)*(strlen(file_location)+2));
  strncpy(temp,file_location,strlen(file_location)-3);
  temp[strlen(file_location)-3] = 'j';
  temp[strlen(file_location)-2] = 's';
  temp[strlen(file_location)-1] = 'o';
  temp[strlen(file_location)] = 'n';
  temp[strlen(file_location)+1] = '\0';
  return temp;
}
