/* PicGuy.cpp
   Principal file

   Copyright (C) 2016 - Arthur M 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>

#include "PhotoFormats.hpp"
#include "PhotoGroup.hpp"

#include "JPEGPhoto.hpp"
#include "PNGPhoto.hpp"

extern "C" {
#include <gtk/gtk.h>
}

enum ColPhotos {
  COL_NAME = 0, NCOLS
};

std::map<gchar*, Photo*> photo_string;
PhotoFormats f;
GtkWidget* main_win;

static void gapp_add_photo_to_list(GtkTreeStore* store, gchar* name){

  Photo* photoInstance = f.GetFormat(strrchr(name, '.'));
  
  if (!photoInstance->Open()){
    //Error while opening the photo
    char* errname = strerror(errno);
    GtkWidget* msgbox = gtk_message_dialog_new(GTK_WINDOW(main_win),
					       GTK_DIALOG_DESTROY_WITH_PARENT,
					       GTK_MESSAGE_ERROR,
					       GTK_BUTTONS_CLOSE,
					       "Error while opening %s.\n%s",
					       name, errname);
    gtk_dialog_run(GTK_DIALOG(msgbox));
    gtk_widget_destroy(msgbox);
    errno = 0;
    return;					       
  }

  
  GtkTreeIter it;
  gtk_tree_store_append(store, &it, NULL);
  gtk_tree_store_set(store, &it, COL_NAME, name, -1);
  
  photo_string.emplace(name, photoInstance); //TODO: Instantiate and add a Photo object here.

}

static void gapp_add_photo_click(GtkButton* btn, gpointer* data){
  /* Create a open dialog to add a photo */
  g_print("Adding photo... \n");

  GtkWidget* dlgOpenFile;
  dlgOpenFile = gtk_file_chooser_dialog_new("Add Photo", GTK_WINDOW(main_win),
			      GTK_FILE_CHOOSER_ACTION_OPEN,
			      "Cancel", GTK_RESPONSE_CANCEL,
			      "Add", GTK_RESPONSE_ACCEPT,
			      NULL);
  gint res = gtk_dialog_run(GTK_DIALOG(dlgOpenFile));
  if (res == GTK_RESPONSE_ACCEPT){
    GtkFileChooser* filechooser = GTK_FILE_CHOOSER(dlgOpenFile);
    gchar* name = gtk_file_chooser_get_filename(filechooser);
    gapp_add_photo_to_list((GtkTreeStore*)data, name);
    g_free(name);
    
  }

  gtk_widget_destroy(dlgOpenFile);
			      
			      
}

/* This function runs when the GTK main loop is run */
static void gapp_activate(GtkApplication* gapp,
			  gpointer data){
  //Widgets
  GtkWidget *treePhotos, *btnAddPhoto;
  treePhotos = gtk_tree_view_new();

  ///Add tree view columns

  GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn* colPhotoName = gtk_tree_view_column_new_with_attributes("Name", renderer,
									     "text", COL_NAME, NULL);
   ///The list store object, used for adding rows to tree view
  GtkTreeStore* storePhotos = gtk_tree_store_new(NCOLS, G_TYPE_STRING);
  
  //Test item to test list insertion
  gtk_tree_view_append_column(GTK_TREE_VIEW(treePhotos), colPhotoName);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treePhotos), GTK_TREE_MODEL(storePhotos));

  //Add photo button
  btnAddPhoto = gtk_button_new_with_label("Add Photo");
  g_signal_connect(btnAddPhoto, "clicked", G_CALLBACK(gapp_add_photo_click), storePhotos);
  
  //Main grid
  GtkWidget* grid;
  grid = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid), treePhotos, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), btnAddPhoto, 0, 1, 1, 1);

  
  //Main window
  main_win = gtk_application_window_new(gapp);

  gtk_window_set_title(GTK_WINDOW(main_win), "PicGuy");
  gtk_container_add(GTK_CONTAINER(main_win), grid);
  gtk_window_maximize(GTK_WINDOW(main_win));
  gtk_widget_show_all(main_win);


}

int main(int argc, char* argv[]){

  photo_string = std::map<gchar*, Photo*>();
  /* Register supported formats */
  f.RegisterFormat(".jpg", new JPEGPhoto());
  f.RegisterFormat(".jpeg", new JPEGPhoto());
  f.RegisterFormat(".png", new PNGPhoto());

  /* Create the GTK application object and main window */
  GtkApplication* gapp;
  gapp = gtk_application_new("arthurmco.picguy",
			     G_APPLICATION_FLAGS_NONE);
  g_signal_connect(gapp, "activate", G_CALLBACK(gapp_activate), NULL);
  int ret = g_application_run(G_APPLICATION(gapp), argc, argv);

  g_object_unref(gapp);

  /* Return the error code gave by GtkApplication */
  return ret;
  
}
