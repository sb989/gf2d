#ifndef __GF2D_GTK_
#define __GF2D_GTK_
#include <gtk/gtk.h>
#include <stdio.h>
#include "gf2d_level_editor.h"
void gf2d_file_directory_open(void * nothing);
void gf2d_file_directory_save(void * nothing);
void gf2d_gtk_init(int arc,char **argv);
char * gf2d_gtk_get_filename();
char * open_dialog();
#endif
