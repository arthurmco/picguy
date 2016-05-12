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

/* Interface helpers */
void add_group_to_tree(GtkTreeView* tree, PhotoGroup* pg);
PhotoGroup* get_selected_group_item();

/* Callbacks */
static void add_folder_activate(GtkWidget* item, gpointer data);

struct PGWidgets {
  GtkBuilder* gbuilder = nullptr;
  GtkWidget* gWinMain = nullptr;

  GtkWidget* treeGroups = nullptr;
  GtkTreeModel* treeModel = nullptr;
} widgets;

struct PGData {
  PhotoGroup* root_group;
  int last_id = 0;
} data;

static void app_activate(GtkApplication* app, gpointer user_data)
{
    widgets.gbuilder = gtk_builder_new_from_file("mainlayout.ui");
    widgets.gWinMain = GTK_WIDGET(gtk_builder_get_object(widgets.gbuilder, "winMain"));

    widgets.treeGroups = GTK_WIDGET(gtk_builder_get_object(widgets.gbuilder, "tvGroups"));
    GtkCellRenderer *treeRenderer;
    GtkTreeViewColumn *column;
    treeRenderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Group name",
                                                   treeRenderer,
                                                   "text", 0,
                                                   NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (widgets.treeGroups), column);

    widgets.treeModel = GTK_TREE_MODEL(gtk_builder_get_object(widgets.gbuilder, "lsTreeGroups"));

    if (!widgets.gWinMain) {
        fprintf(stderr, "Cannot load main window layout!");
        exit(EXIT_FAILURE);
    }

    gtk_application_add_window(app, GTK_WINDOW(widgets.gWinMain));

    GtkWidget* gmAddFolder = GTK_WIDGET(
        gtk_builder_get_object(widgets.gbuilder, "itemAddFolder"));
    g_signal_connect(gmAddFolder, "activate",
        G_CALLBACK(add_folder_activate), NULL);

    /* Initialize the data */
    data.root_group = new PhotoGroup{"Root", data.last_id++};

    gtk_window_maximize(GTK_WINDOW(widgets.gWinMain));
    gtk_widget_show_all(widgets.gWinMain);
}

void add_group_to_tree(GtkTreeView* tree, PhotoGroup* pg)
{
  GtkTreeModel* model = widgets.treeModel;

  if (!model) {
      fprintf(stderr, "error: model not found.\n");
  }

  GtkTreeStore* store = GTK_TREE_STORE(model);

  GtkTreeIter iter;
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 0, pg->GetName(), -1);

  printf(" created item for %s, ", pg->GetName());

}

PhotoGroup* get_selected_group_item()
{
  GtkTreeSelection* treeSel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widgets.treeGroups));
  return nullptr;
}

static void add_folder_activate(GtkWidget* item, gpointer user_data)
{
    fprintf(stderr, "Adding folder... ");
    GtkWidget* folderSelector = gtk_file_chooser_dialog_new(
      "Select folder", GTK_WINDOW(widgets.gWinMain), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      "_Cancel", GTK_RESPONSE_CANCEL,
      "_Add", GTK_RESPONSE_ACCEPT, NULL);

    gint result = gtk_dialog_run(GTK_DIALOG(folderSelector));
    if (result == GTK_RESPONSE_ACCEPT) {
      char* foldername = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(folderSelector));
      printf(" added %s, ", foldername);

      PhotoGroup* group = new PhotoGroup{foldername, data.last_id++};

      /* add folder */
      PhotoGroup* parent_group = get_selected_group_item();
      if (!parent_group)
        parent_group = data.root_group;

      parent_group->AddPhotoGroup(group);

      add_group_to_tree(GTK_TREE_VIEW(widgets.treeGroups), group);

      /* won't be needed anymore */
      gtk_widget_destroy(folderSelector);
    }
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
