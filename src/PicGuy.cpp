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

#include "ThumbnailCache.hpp"

extern "C" {
#include <gtk/gtk.h>
}

#include <dirent.h>

/* Interface helpers */
void add_group_to_tree(GtkTreeView* tree, PhotoGroup* pg, GtkTreeIter* parent);
PhotoGroup* get_selected_group_item(GtkTreeIter* it);

void add_photo_to_list(Photo* photo, GdkPixbuf* thumbnail);

/* Callbacks */
static void add_folder_activate(GtkWidget* item, gpointer data);
static void tree_groups_row_activated(GtkTreeView* tv, GtkTreePath* path,
    GtkTreeViewColumn* col, gpointer data);


struct PGWidgets {
  GtkBuilder* gbuilder = nullptr;
  GtkWidget* gWinMain = nullptr;

  GtkWidget* treeGroups = nullptr;
  GtkTreeModel* treeModel = nullptr;

  GtkWidget* listPhotos = nullptr;
  GtkListStore* listModel = nullptr;
} widgets;

struct PGData {
  PhotoGroup* root_group;
  PhotoFormats* photo_formats;
  ThumbnailCache* tc;
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
    g_signal_connect(widgets.treeGroups, "row-activated",
        G_CALLBACK(tree_groups_row_activated), NULL);


    widgets.treeModel = GTK_TREE_MODEL(gtk_builder_get_object(widgets.gbuilder, "lsTreeGroups"));

    widgets.listPhotos = GTK_WIDGET(gtk_builder_get_object(widgets.gbuilder, "ivPhotos"));
    gtk_icon_view_set_pixbuf_column (GTK_ICON_VIEW (widgets.listPhotos), 0);
    gtk_icon_view_set_text_column (GTK_ICON_VIEW (widgets.listPhotos), 1);

    widgets.listModel = GTK_LIST_STORE(gtk_builder_get_object(widgets.gbuilder, "lsPhotos"));

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

    GError* err;

    if (err) {
      printf("Error: %s\n", err->message);
    }
}

void add_group_to_tree(GtkTreeView* tree, PhotoGroup* pg, GtkTreeIter* parent)
{
  GtkTreeModel* model = widgets.treeModel;

  if (!model) {
      fprintf(stderr, "error: model not found.\n");
  }

  GtkTreeStore* store = GTK_TREE_STORE(model);

  GtkTreeIter iter;
  gtk_tree_store_append(store, &iter, parent);
  gtk_tree_store_set(store, &iter, 0, pg->GetName(), -1);

  printf(" created item for %s, ", pg->GetName());

}

PhotoGroup* get_selected_group_item(GtkTreeIter* out_iter)
{
  GtkTreeSelection* treeSel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widgets.treeGroups));
  GtkTreeIter it;

  if (gtk_tree_selection_get_selected(treeSel,
      &widgets.treeModel, &it) == TRUE) {
        gchar* name;
        gtk_tree_model_get(widgets.treeModel, &it, 0, &name, -1);

        PhotoGroup* grp = data.root_group->GetPhotoGroup(name);
        if (grp) {
          fprintf(stderr, "Group found: %s (id %d)\n", grp->GetName(), grp->GetID());

          if (out_iter)
            *out_iter = it;

          return grp;
        }
        fprintf(stderr, "Group not found for %s\n", name);

  }

  fprintf(stderr, "Not found\n");
  return nullptr;


}

void add_photo_to_list(Photo* photo, GdkPixbuf* thumbnail)
{

  /* Get only the file name */
    std::string pname = std::string{photo->GetName()};
    pname = pname.substr(pname.find_last_of('/')+1);

    GtkTreeIter iter;
    gtk_list_store_append(widgets.listModel, &iter);
    gtk_list_store_set(widgets.listModel, &iter,
        0, thumbnail, 1, pname.c_str(), -1);

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

      /* Retrieve all photos from that folder */
      DIR* photo_dir = opendir(foldername);

      if (!photo_dir) {
          fprintf(stderr, "Unable to open directory %s\n", foldername);
          return;
      }

      struct dirent* dentry;

      while (dentry = readdir(photo_dir)) {
          std::string name = std::string{dentry->d_name};
          int ext_index = name.find_last_of('.');

          /* Check file type. Do not add directories, nor hidden files */
          if (dentry->d_name[0] == '.') {
              continue; //Hidden file.
          }

          if (dentry->d_type == DT_DIR) {
              continue; //DIrectory
          }

          std::string extension;

          if (ext_index != std::string::npos)
            extension = name.substr(ext_index);
          else
            extension = "";

          Photo* photo;
          if (!(photo = data.photo_formats->GetFormat(extension.c_str()))) {
                continue; //Invalid extension.
          }

          printf("\tFound %s, extension %s\n", name.c_str(), extension.c_str());
          std::string fullpath = std::string{foldername};
          fullpath.append("/");
          fullpath.append(name);
          photo->SetName(fullpath.c_str());


          group->AddPhoto(photo);

      }

      closedir(photo_dir);

      /* add folder */
      GtkTreeIter* folder = NULL;
      PhotoGroup* parent_group = nullptr; //get_selected_group_item(folder);
      if (!parent_group)
        parent_group = data.root_group;

      parent_group->AddPhotoGroup(group);

      add_group_to_tree(GTK_TREE_VIEW(widgets.treeGroups), group, NULL);

      /* won't be needed anymore */
      gtk_widget_destroy(folderSelector);
    }
}

static void tree_groups_row_activated(GtkTreeView* tv, GtkTreePath* path,
    GtkTreeViewColumn* col, gpointer user_data)
{
    fprintf(stderr, "\n >>> Row selected: ");
    int idx = gtk_tree_path_get_indices(path)[0];
    PhotoGroup* grp = data.root_group->GetPhotoGroupByIndex(idx);
    fputs(grp->GetName(), stderr);

    /* Fill the icon view with the photos */
    gtk_list_store_clear(widgets.listModel);

    grp->ResetPhotoIterator();
    Photo* p;

    while (p = grp->GetNextPhoto()) {
      /* Check if photo is on thumbnail, add it if it doesn't */
      GdkPixbuf* pb; // = data.tc->GetByID(p->GetID());
    //  if (!pb) {
        data.tc->Add(p);
        pb = data.tc->GetByID(p->GetID());
      //}

      add_photo_to_list(p, pb);
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

  data.photo_formats = new PhotoFormats{};
  data.photo_formats->RegisterFormat(".jpg", new JPEGPhoto{});
  data.photo_formats->RegisterFormat(".jpeg", new JPEGPhoto{});

  data.tc = new ThumbnailCache{};

  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
