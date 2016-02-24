/* PicGuy.cpp
   Principal file

   Copyright (C) 2016 - Arthur M 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* This function runs when the GTK main loop is run */
static void gapp_activate(GtkApplication* gapp,
			  gpointer data){
  //Widgets
  GtkWidget *treePhotos, *btnAddPhoto;
  treePhotos = gtk_tree_view_new();

  ///Add tree view columns

  GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn* colPhotoName = gtk_tree_view_column_new_with_attributes("Name", renderer,
									     COL_NAME, "cname", NULL);
   ///The list store object, used for adding rows to tree view
  GtkTreeStore* storePhotos = gtk_tree_store_new(NCOLS, G_TYPE_STRING);
  
  //Test item to test list insertion
  
  GtkTreeIter it;
  gtk_tree_store_append(storePhotos, &it, NULL);
  gtk_tree_store_set(storePhotos, &it, COL_NAME, "Teste", -1);

  GtkTreeIter it2;
  gtk_tree_store_append(storePhotos, &it2, NULL);
  gtk_tree_store_set(storePhotos, &it2, COL_NAME, "Test.jpg", -1);

  gtk_tree_view_append_column(GTK_TREE_VIEW(treePhotos), colPhotoName);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treePhotos), GTK_TREE_MODEL(storePhotos));

  
  btnAddPhoto = gtk_button_new_with_label("Add Photo");
  
  //Main grid
  GtkWidget* grid;
  grid = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid), treePhotos, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), btnAddPhoto, 0, 1, 1, 1);

  
  //Main window
  GtkWidget* main_win;
  main_win = gtk_application_window_new(gapp);

  gtk_window_set_title(GTK_WINDOW(main_win), "PicGuy");
  gtk_container_add(GTK_CONTAINER(main_win), grid);
  gtk_window_maximize(GTK_WINDOW(main_win));
  gtk_widget_show_all(main_win);


}

int main(int argc, char* argv[]){

  PhotoFormats f;

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
