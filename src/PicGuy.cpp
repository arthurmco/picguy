/* PicGuy.cpp
   Principal file

   Copyright (C) 2016 - Arthur M
*/

#include <stdio.h>
#include <stdlib.h> //exit()
#include <string.h>

#include <map>

#include "PhotoFormats.hpp"
#include "PhotoGroup.hpp"

#include "JPEGPhoto.hpp"
#include "PNGPhoto.hpp"

extern "C" {
#include <gtk/gtk.h>
}

#include <dirent.h>


struct PGWidgets {
  GtkBuilder* gbuilder = nullptr;
  GtkWidget* gWinMain = nullptr;
} widgets;

static void app_activate(GtkApplication* app, gpointer user_data)
{
    widgets.gbuilder = gtk_builder_new_from_file("mainlayout.ui");
    widgets.gWinMain = GTK_WIDGET(gtk_builder_get_object(widgets.gbuilder, "winMain"));

    if (!widgets.gWinMain) {
        fprintf(stderr, "Cannot load main window layout!");
        exit(EXIT_FAILURE);
    }

    gtk_application_add_window(app, GTK_WINDOW(widgets.gWinMain));
    gtk_window_maximize(GTK_WINDOW(widgets.gWinMain));
    gtk_widget_show_all(widgets.gWinMain);
}


int main(int argc, char* argv[])
{
  GtkApplication* app;
  int status;

  /* TODO: Change to G_APPLICATION_HANDLES_CMDLINE (or something)
     as soon as we can handle them */
  app = gtk_application_new("com.arthurmco.picguy", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);

  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
