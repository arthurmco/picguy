#include "ThumbnailCache.hpp"

int ThumbnailCache::Add(Photo* ph) {
  /* Open the image, resize it and put it in a GdkPixbuf */
  if (!ph->Open()) {
      return -1;
  }

  Pixel* pixel = ph->GetRawData();

  if (!pixel) {
    return -1;
  }

  printf("Creating thumbnail of %s (id %d) ", ph->GetName(), ph->GetID() );

  int width = ph->GetWidth();
  int height = ph->GetHeight();

  float divisor = (width > height) ? width / DEFAULT_THUMBNAIL_SIZE : height / DEFAULT_THUMBNAIL_SIZE;

  int thumb_width = int(width / divisor);
  int thumb_height = int(height / divisor);
  int has_alpha = (ph->GetBitDepth() >= 32) ? TRUE : FALSE;
  int stride = (has_alpha == TRUE) ?
      ph->GetWidth() * 4 : ph->GetWidth() * 3;

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
