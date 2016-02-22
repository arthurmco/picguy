/* PhotoGroup.hpp
   Manages groups of photos.

   Copyright (C) 2016 - Arthur M 
*/


#ifndef PHOTOGROUP_HPP
#define PHOTOGROUP_HPP

#include <vector>

#include "Photo.hpp"

class PhotoGroup {
private:
  std::vector<Photo*> _photos;

public:
  /* Adds a photo, returns its ID number */
  int AddPhoto(Photo*);

  /* Removes the photo whose ID is 'id' */
  void RemovePhoto(int id);

  Photo* GetPhoto(int id);
  
  /* Get the photo count */
  int GetPhotoCount();


  /* Get the photo from a directory
     TODO: Maybe take this function out of here */
  static PhotoGroup* GetFromDirectory(const char* dirname);
  

};


#endif
