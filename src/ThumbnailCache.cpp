#include "ThumbnailCache.hpp"

int ThumbnailCache::Add(Photo* ph) {
  if (cache_entries.find(ph->GetID()) != cache_entries.end()) {
      return ph->GetID();   // Already exists, return only ID.
  }

  /* Open the image, resize it and put it in a GdkPixbuf */
  if (!ph->Open()) {
      g_warning("Error while opening %s: %d (%s)", ph->GetName(), errno,
        strerror(errno));
      return -1;
  }

  Pixel* pixel;
  try {
    pixel = ph->GetThumbnail();
  } catch (std::runtime_error& exc) {
    g_warning("Error while getting pixel data of %s: %s",
        ph->GetName(), exc.what());

    pixel = nullptr;
    return 0;
  }

  if (!pixel) {

    ThumbnailData t;
    t.photo = ph;
    GtkWidget* w = gtk_image_new_from_icon_name ("gtk-missing-image", GTK_ICON_SIZE_LARGE_TOOLBAR);
    t.pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(w));
    gtk_widget_destroy(w);
    int id = ph->GetID();
    cache_entries[id] = t;
    return id;
  }

  printf("Creating thumbnail of %s (id %d) (%dx%d) ", ph->GetName(), ph->GetID(),
    ph->GetThumbnailWidth(), ph->GetThumbnailHeight());

  int width = ph->GetThumbnailWidth();
  int height = ph->GetThumbnailHeight();

  float divisor = (width > height) ? width / DEFAULT_THUMBNAIL_SIZE : height / DEFAULT_THUMBNAIL_SIZE;
  int thumb_width = int(width / divisor);
  int thumb_height = int(height / divisor);
  int has_alpha = TRUE;
  int stride = ph->GetThumbnailWidth()*4;   // now thumbnails always have 32bpp

  GdkPixbuf* pb = gdk_pixbuf_new_from_data((guchar*) pixel,
    GDK_COLORSPACE_RGB, has_alpha, 8, width, height, stride, NULL, NULL);

  GdkPixbuf* pb_thumb = gdk_pixbuf_scale_simple(pb, thumb_width, thumb_height,
    GDK_INTERP_BILINEAR);

  ThumbnailData t;
  t.photo = ph;
  t.pixbuf = pb_thumb;

  printf(", created thumbnail of %dx%d, at %p\n", thumb_width, thumb_height, pb_thumb );

  int id = ph->GetID();

  g_object_unref(pb);
  ph->FreeRawData();
  ph->Close();

  cache_entries[id] = t;

  return id;
}
void ThumbnailCache::Remove(int id) {
  /* Destroy the thumbnail and remove the image */
  if (cache_entries.find(id) != cache_entries.end()) {
    g_object_unref(cache_entries[id].pixbuf);
    delete cache_entries[id].photo;
    cache_entries.erase(id);
  }
}

GdkPixbuf* ThumbnailCache::GetByID(int id) {
  if (cache_entries.find(id) == cache_entries.end()) return nullptr;
  return cache_entries[id].pixbuf;
}
