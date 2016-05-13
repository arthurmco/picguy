/* Thumbnail cache controling

   Copyright (C) 2016 - Arthur M
*/

#include <map>

extern "C" {
#include <gtk/gtk.h>
}

#include "Photo.hpp"

#ifndef THUMBNAILCACHE_HPP
#define THUMBNAILCACHE_HPP

struct ThumbnailData {
  Photo* photo;
  GdkPixbuf* pixbuf;
};

class ThumbnailCache {
private:
  std::map<int /* id */, ThumbnailData /* thumbnail */> cache_entries;

  #define MAX_THUMB_COUNT 100
  #define DEFAULT_THUMBNAIL_SIZE 96
  int last_used_ids[MAX_THUMB_COUNT];
  int last_used_ptr = 0;

public:
  int Add(Photo*);
  void Remove(int id);

  /* Return the pixbuf of thumb whose photo ID is 'id', or nullptr if
    it doesn't exist */
  GdkPixbuf* GetByID(int id);

};



#endif /* end of include guard: THUMBNAILCACHE_HPP */
