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
void fill_list_with_photos(PhotoGroup* grp);

/* Callbacks */
static void add_folder_activate(GtkWidget* item, gpointer data);
static void add_photo_activate(GtkWidget* item, gpointer data);
static void tree_groups_row_activated(GtkTreeView* tv, GtkTreePath* path,
    GtkTreeViewColumn* col, gpointer data);
static void icon_images_row_activated(GtkIconView* icon, GtkTreePath* path,
    gpointer userdata);


struct PGWidgets {
  GtkBuilder* gbuilder = nullptr;
  GtkWidget* gWinMain = nullptr;

  GtkWidget* treeGroups = nullptr;
  GtkTreeModel* treeModel = nullptr;

  GtkWidget* listPhotos = nullptr;
  GtkListStore* listModel = nullptr;

  GtkWidget* widExpInfo;
  GtkWidget* widImage;
  GtkWidget* widImageName;
  GtkWidget* widImageArea;
  GtkWidget* widImageSize;
  GtkWidget* widImageMime;
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
    g_signal_connect(widgets.listPhotos, "item-activated",
        G_CALLBACK(icon_images_row_activated), NULL);

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

    GtkWidget* gmAddPhoto = GTK_WIDGET(
        gtk_builder_get_object(widgets.gbuilder, "itemAddPhoto"));
    g_signal_connect(gmAddPhoto, "activate",
        G_CALLBACK(add_photo_activate), NULL);

    /* Initialize the data */
    data.root_group = new PhotoGroup{"Root", data.last_id++};

    /* Get the information widgets */
    widgets.widImage = GTK_WIDGET(
        gtk_builder_get_object(widgets.gbuilder, "imgThumbnail"));
    widgets.widImageName = GTK_WIDGET(
        gtk_builder_get_object(widgets.gbuilder, "lblImageName"));
    widgets.widImageArea = GTK_WIDGET(
        gtk_builder_get_object(widgets.gbuilder, "lblImageArea"));
    widgets.widImageSize = GTK_WIDGET(
        gtk_builder_get_object(widgets.gbuilder, "lblImageSize"));
    widgets.widImageMime = GTK_WIDGET(
        gtk_builder_get_object(widgets.gbuilder, "lblImageType"));
    widgets.widExpInfo = GTK_WIDGET(
        gtk_builder_get_object(widgets.gbuilder, "expInfo"));


    gtk_window_maximize(GTK_WINDOW(widgets.gWinMain));
    gtk_widget_show_all(widgets.gWinMain);
  //  gtk_widget_set_visible(widgets.widExpInfo, FALSE);

    GError* err = NULL;

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

          if (!photo->Open()) {
              continue;
          }

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

void fill_list_with_photos(PhotoGroup* grp)
{
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

static void tree_groups_row_activated(GtkTreeView* tv, GtkTreePath* path,
    GtkTreeViewColumn* col, gpointer user_data)
{
    fprintf(stderr, "\n >>> Row selected: ");
    int idx = gtk_tree_path_get_indices(path)[0];
    PhotoGroup* grp = data.root_group->GetPhotoGroupByIndex(idx);
    fill_list_with_photos(grp);
}

static void icon_images_row_activated(GtkIconView* icon, GtkTreePath* path,
    gpointer userdata)
{
  GdkPixbuf* gp = gtk_image_get_pixbuf (GTK_IMAGE(widgets.widImage));
  if (gp) g_object_unref(gp);

  //gtk_widget_set_visible(widgets.widExpInfo, TRUE);

  fprintf(stderr, "\n >>> Image selected: ");

  if (!path) return;

  int idx = gtk_tree_path_get_indices(path)[0];

  PhotoGroup* grp = get_selected_group_item(NULL);
  if (!grp) return;

  Photo* photo = grp->GetPhotoByIndex(idx);
  if (!photo) return;

  puts(photo->GetName());

  char msg_area[32];
  char msg_size[32];
  sprintf(msg_area, "Area: %d x %d", photo->GetWidth(), photo->GetHeight());

  float unitcount = photo->GetSize();
  const char* unitdata[] = {"bytes", "KB", "MB", "GB", "TB", "EB"};
  int unitind = 0;

  while (unitcount >= 1024.0f) {
      unitcount /= 1024.0f;
      unitind++;
  }


  /* Resize the image to fit into the widget in 'informations' expander */
  Pixel* p;

  try {
    p = photo->GetRawData();
  } catch (std::runtime_error& exc) {
    g_warning("Error while opening %s: %s", photo->GetName(), exc.what());
    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkWidget* dialog = gtk_message_dialog_new (GTK_WINDOW(widgets.gWinMain),
                                 flags,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_OK,
                                 "Error reading “%s”: %s",
                                 photo->GetName(),
                                 exc.what());
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    return;
  }

  if (!p) return;

  sprintf(msg_size, "Size: %.3f %s", unitcount, unitdata[unitind]);

  printf("\n\t %s", msg_area);
  printf("\n\t %s", msg_size);
  printf("\n\t %d bpp\n", photo->GetBitDepth());

  gtk_label_set_text(GTK_LABEL(widgets.widImageName), photo->GetName());
  gtk_label_set_text(GTK_LABEL(widgets.widImageArea), msg_area);
  gtk_label_set_text(GTK_LABEL(widgets.widImageSize), msg_size);

  float divisor = (photo->GetWidth() > photo->GetHeight()) ?
      photo->GetWidth() / 160.0 : photo->GetHeight() / 160.0;

  int thumb_width = int(photo->GetWidth() / divisor);
  int thumb_height = int(photo->GetHeight() / divisor);
  int has_alpha = (photo->GetBitDepth() >= 32) ? TRUE : FALSE;
  int stride = (has_alpha == TRUE) ?
      photo->GetWidth() * 4 : photo->GetWidth() * 3;

  GdkPixbuf* pb = gdk_pixbuf_new_from_data((guchar*) p,
    GDK_COLORSPACE_RGB, has_alpha, 8, photo->GetWidth(), photo->GetHeight(),
    stride, NULL, NULL);

  GdkPixbuf* pb_thumb = gdk_pixbuf_scale_simple(pb, thumb_width, thumb_height,
    GDK_INTERP_BILINEAR);

  g_object_unref(pb);
  photo->FreeRawData();

  gtk_image_set_from_pixbuf(GTK_IMAGE(widgets.widImage), pb_thumb);

}

static void add_photo_activate(GtkWidget* item, gpointer user_data) {
  GtkWidget* dlgNewPhoto = gtk_file_chooser_dialog_new ("Add Photos",
                                      GTK_WINDOW(widgets.gWinMain),
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      "_Cancel",
                                      GTK_RESPONSE_CANCEL,
                                      "_Add",
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dlgNewPhoto), TRUE);

  int index = 0;
  std::vector<char*> fmts = data.photo_formats->GetFormats();

  GtkFileFilter* ft_all = gtk_file_filter_new();
  gtk_file_filter_set_name(ft_all, "All supported formats");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlgNewPhoto), ft_all);

  for (auto it = fmts.begin(); it != fmts.end(); ++it) {
      GtkFileFilter* ft = gtk_file_filter_new();

      char fmtname[30], fmtext[30];
      sprintf(fmtext, "*%s", *it);
      sprintf(fmtname, "%s files (%s)",
        data.photo_formats->GetFormat(*it)->GetType(), fmtext);

      gtk_file_filter_set_name(ft, fmtname);
      gtk_file_filter_add_pattern(ft, fmtext);
      gtk_file_filter_add_pattern(ft_all, fmtext);
      gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlgNewPhoto), ft);
  }


    gint res = gtk_dialog_run (GTK_DIALOG (dlgNewPhoto));
    bool group_selected = true;

    if (res == GTK_RESPONSE_ACCEPT) {
      /* Get the selected group, or create one */
      PhotoGroup* pg = get_selected_group_item(NULL);
      if (!pg) {
        group_selected = false;
        pg = new PhotoGroup{"Images", data.last_id++};
        add_group_to_tree(GTK_TREE_VIEW(widgets.treeGroups), pg, NULL);
      }

      /* Get all the files, try to open them and add them */
      GSList* files = gtk_file_chooser_get_filenames(
        GTK_FILE_CHOOSER(dlgNewPhoto));

      GSList* file_next = files;


      do {
          gchar* fname = (gchar*) file_next->data;
          printf("\nAdding %s", fname);

          std::string name = std::string{fname};
          int ext_index = name.find_last_of('.');

          std::string extension;

          if (ext_index != std::string::npos)
            extension = name.substr(ext_index);
          else
            extension = "";

          Photo* photo = data.photo_formats->GetFormat(extension.c_str());

          if (!photo) {
            //Not compatible (weird);
            g_warning("%s isn't supported (weird)", fname);
            continue;
          }

          photo->SetName(name.c_str());
          if (!photo->Open()) {
            //Opening error.
            g_warning("%s couldn't be opened", name.c_str());
            continue;
          }

          //Add photo
          pg->AddPhoto(photo);
      } while (file_next = g_slist_next(file_next));

      data.root_group->AddPhotoGroup(pg);
      if (group_selected) {
        /* Update group view */
        fill_list_with_photos(pg);

      }
    }

    gtk_widget_destroy (dlgNewPhoto);
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
  data.photo_formats->RegisterFormat(".png", new PNGPhoto{});

  data.tc = new ThumbnailCache{};

  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
