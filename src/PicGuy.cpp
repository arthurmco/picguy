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

#include <dirent.h>

enum ColPhotos {
  COL_NAME = 0, NCOLS
};

/* Type of item in the photo listing tree view */
enum PhotoTreeItemType {
  LIST_PHOTO,
  LIST_PHOTOGROUP,
};

struct PhotoListInfo {
  int type;
  union {
    Photo* photo;
    PhotoGroup* group;
  } item;
};

/* Container of all widgets this window have */
struct {
  GtkWidget* lblPhotoInformation = NULL;
  GtkWidget *treePhotos, *btnAddPhoto, *btnAddFolder;
} widgets;

std::map<std::string, PhotoListInfo> photo_string;
PhotoFormats f;

GtkWidget* main_win;

static int gapp_add_photo_to_list(GtkTreeStore* store, gchar* name, GtkTreeIter* parent,
				  PhotoGroup* grp = NULL){  
  Photo* photoInstance = f.GetFormat(strrchr(name, '.'));

  if (!photoInstance){

    return EINVAL;
  }
  
  photoInstance->SetName(name);
  if (!photoInstance->Open()){
     
    return errno;					       
  }

  
  /* Search the file name and add it if it doesn't exist
     Unfortunately I will have to iterate.
     The count() element doesn't seems to work with strings */
  bool found = false;
  for (auto ph_it = photo_string.begin(); ph_it != photo_string.end(); ph_it++){

    if (!ph_it->first.compare(std::string(name))){
      
      found = true;
      break;
    }
  }

  if (!found){

    PhotoListInfo pli;
    pli.type = LIST_PHOTO;
    pli.item.photo = photoInstance;

    if (grp)
      grp->AddPhoto(photoInstance);
    
    photo_string.insert(std::pair<std::string, PhotoListInfo>
			(std::string(name), pli)); //TODO: Instantiate and add a Photo object here.
      
      GtkTreeIter it;
      gtk_tree_store_append(store, &it, parent);
      gtk_tree_store_set(store, &it, COL_NAME, name, -1);
      g_print("Photo added: %s\n", name);
      return 0;
  }

  /* File is already there.
     Return EEXIST */ 
  return EEXIST;

  
}

static void gapp_add_folder_click(GtkButton* btn, gpointer data){
  g_print("Adding folder... \n");

  GtkWidget* dlgOpenDir;
  dlgOpenDir = gtk_file_chooser_dialog_new("Add Folder", GTK_WINDOW(main_win),
					   GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
					   "Cancel", GTK_RESPONSE_CANCEL,
					   "Add", GTK_RESPONSE_ACCEPT,
					   NULL);

  gint res = gtk_dialog_run(GTK_DIALOG(dlgOpenDir));
  if (res == GTK_RESPONSE_ACCEPT){
    GtkFileChooser* filechooser = GTK_FILE_CHOOSER(dlgOpenDir);
    gchar* folder = gtk_file_chooser_get_filename(filechooser);

    /* Add all files from that directory in the list
       TODO: Separate them in photo groups */
    DIR* dir = opendir(folder);

    if (!dir){
      char* errname = strerror(errno);
      GtkWidget* msgbox = gtk_message_dialog_new(GTK_WINDOW(main_win),
						 GTK_DIALOG_DESTROY_WITH_PARENT,
						 GTK_MESSAGE_ERROR,
						 GTK_BUTTONS_CLOSE,
						 "Error while opening directory %s.\n%s",
						 folder, errname);
      gtk_dialog_run(GTK_DIALOG(msgbox));
      gtk_widget_destroy(msgbox);
      return;
   
    }
    
    struct dirent* dirent; 

    PhotoGroup* grp = new PhotoGroup(folder);
    
    PhotoListInfo pli;
    pli.type = LIST_PHOTOGROUP;
    pli.item.group = grp;
    
    photo_string.insert(std::pair<std::string, PhotoListInfo>
			(std::string(folder), pli)); //TODO: Instantiate and add a Photo object here.


    
    GtkTreeIter it;
    gtk_tree_store_append((GtkTreeStore*)data, &it, NULL);
    gtk_tree_store_set((GtkTreeStore*)data, &it, COL_NAME, folder, -1);
    bool errors = false;
    std::string error_desc = "";
    
    while (dirent = readdir(dir)){
      //Is the directory entry a file?
      if (dirent->d_type == DT_REG){

	//Add a tree item representing the directory
	g_print("Adding child file %s\n", dirent->d_name);

	std::string fullpath = std::string(folder);
	fullpath.append("/");
	fullpath.append(dirent->d_name);
	int ret_add = gapp_add_photo_to_list((GtkTreeStore*)data, (gchar*)fullpath.c_str(), &it, grp);
	
	if (ret_add != 0){
	  errors = true;
	  error_desc.append(fullpath);
	  error_desc.append(": ");
	  error_desc.append(strerror(ret_add));
	  error_desc.append("\n");
	  errno = 0;
	}

      }

    }

    closedir(dir);

    if (errors){
      GtkWidget* msgbox = gtk_message_dialog_new(GTK_WINDOW(main_win),
						 GTK_DIALOG_DESTROY_WITH_PARENT,
						 GTK_MESSAGE_ERROR,
						 GTK_BUTTONS_CLOSE,
						 "The following errors happened:\n\n%s",
						 error_desc.c_str());
      gtk_dialog_run(GTK_DIALOG(msgbox));
      gtk_widget_destroy(msgbox);

    }

  }


  gtk_widget_destroy(dlgOpenDir);

  
}

static void gapp_add_photo_click(GtkButton* btn, gpointer data){
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

    int ret_add = gapp_add_photo_to_list((GtkTreeStore*)data, name, NULL);
    
    if (ret_add != 0){
      //Error occurred.
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
    }
    
  }

  gtk_widget_destroy(dlgOpenFile);
			      
			      
}


static void gapp_tree_rowactivated(GtkTreeView* treeView, GtkTreePath* path,
				   GtkTreeViewColumn* col, GtkTreeStore* store){
  
  gchar* pname;
  GValue pval = G_VALUE_INIT;
  
  /* Get the iterator for the item */
  GtkTreeIter it;
  gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &it, path);
  gtk_tree_model_get_value(GTK_TREE_MODEL(store), &it, COL_NAME, &pval);
  pname = *(gchar**)pval.data;
  
  g_print("Row clicked %s\n", pname);

  /* Retrieve name and search in photo_string map */
  std::string name(pname);
  auto it_photo = photo_string.find(name);
  g_print("%d \n", it_photo->second.type);

  char message[128];

  switch (it_photo->second.type){
  case LIST_PHOTO:
    sprintf(message, "%s, %d x %d pixels",
	    name.substr(name.find_last_of('/')+1, std::string::npos).c_str(),
	    it_photo->second.item.photo->GetWidth(),
	    it_photo->second.item.photo->GetHeight());
    break;
  case LIST_PHOTOGROUP:
    sprintf(message, "%s, %d photos",
	    name.c_str(), it_photo->second.item.group->GetPhotoCount());
    break;
  }

  gtk_label_set_text(GTK_LABEL(widgets.lblPhotoInformation), message);
  
}



/* This function runs when the GTK main loop is run */
static void gapp_activate(GtkApplication* gapp,
			  gpointer data){
  //Widgets

  widgets.treePhotos = gtk_tree_view_new();

  ///Add tree view columns

  GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn* colPhotoName = gtk_tree_view_column_new_with_attributes("Name", renderer,
									     "text", COL_NAME, NULL);
   ///The list store object, used for adding rows to tree view
  GtkTreeStore* storePhotos = gtk_tree_store_new(NCOLS, G_TYPE_STRING);
  
  //Test item to test list insertion
  gtk_tree_view_append_column(GTK_TREE_VIEW(widgets.treePhotos), colPhotoName);
  gtk_tree_view_set_model(GTK_TREE_VIEW(widgets.treePhotos), GTK_TREE_MODEL(storePhotos));  
  //Add photo button
  widgets.btnAddPhoto = gtk_button_new_with_label("Add Photo");
  g_signal_connect(widgets.btnAddPhoto, "clicked", G_CALLBACK(gapp_add_photo_click), storePhotos);

  widgets.btnAddFolder = gtk_button_new_with_label("Add Folder");
  g_signal_connect(widgets.btnAddFolder, "clicked", G_CALLBACK(gapp_add_folder_click), storePhotos);

  //Setup the label with photo information
  widgets.lblPhotoInformation = gtk_label_new("Please select a photo");

  
  g_signal_connect(widgets.treePhotos, "row-activated", G_CALLBACK(gapp_tree_rowactivated), storePhotos);
  
  //Main grid
  GtkWidget* grid;
  grid = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid), widgets.treePhotos, 0, 0, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), widgets.btnAddPhoto, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), widgets.btnAddFolder, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), widgets.lblPhotoInformation, 0, 2, 1, 2);

  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  
  //Main window
  main_win = gtk_application_window_new(gapp);

  gtk_window_set_title(GTK_WINDOW(main_win), "PicGuy");
  gtk_container_add(GTK_CONTAINER(main_win), grid);
  gtk_window_maximize(GTK_WINDOW(main_win));
  gtk_widget_show_all(main_win);


}

int main(int argc, char* argv[]){

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
