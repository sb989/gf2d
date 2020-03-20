
#include "gf2d_gtk.h"
#include <string.h>
#include "simple_logger.h"
#include "gf2d_controls_state.h"
/* Get the selected filename and print it to the console */


static char * filename = NULL;

static char * open_dialog()
{
  GtkWidget *dialog;
  char *file;
  dialog = gtk_file_chooser_dialog_new ("Open File",
    NULL,
    GTK_FILE_CHOOSER_ACTION_OPEN,
    ("_Cancel"),
    GTK_RESPONSE_CANCEL,
    ("_Open"),
    GTK_RESPONSE_ACCEPT,
    NULL);

  slog("pog");
  gtk_widget_show_all(dialog);
//gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),"/");
  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_home_dir());
  gint resp = gtk_dialog_run(GTK_DIALOG(dialog));

  if(resp == GTK_RESPONSE_ACCEPT)
  {
    file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    g_print("%s\n", file);
    gtk_widget_destroy(dialog);
    return file;
  }
  else
  {
    g_print("You pressed Cancel\n");
    gtk_widget_destroy(dialog);
    file = "cancel";
    return file;
  }

}

void gf2d_file_directory_open(void * nothing)
{
  int i;
  gf2d_set_open_file(TRUE);
  filename = open_dialog();
  for (i = 0; i<8;i++)
    gtk_main_iteration();

  gf2d_set_open_file(FALSE);
}


void gf2d_gtk_init(int argc,char **argv)
{
  gtk_init(&argc, &argv);
}

char * gf2d_gtk_get_filename()
{
  return filename;
}
