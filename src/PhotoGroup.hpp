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
  PhotoGroup* _parent;
  std::vector<Photo*> _photos;
  std::vector<PhotoGroup*> _groups;

  std::string _name;
  int _id;

public:
  PhotoGroup(const char* name, int id);

  /* Adds a photo, returns its ID number */
  int AddPhoto(Photo*);

  /* Adds a group */
  int AddPhotoGroup(PhotoGroup*);

  /* Removes the photo whose ID is 'id' */
  void RemovePhoto(int id);

  Photo* GetPhoto(int id);
  Photo* GetPhotoByIndex(int index);

  PhotoGroup* GetPhotoGroup(const char* name);
  PhotoGroup* GetPhotoGroupByIndex(int idx);

  /* Get the photo count */
  int GetPhotoCount();

  /* Get/set the group name */
  void SetName(const char* name);
  char* GetName() const;

  /* Get the parent */
  PhotoGroup* GetParent() const;

  /* Get ID */
  int GetID() const;

  /* Get the photo from a directory
     TODO: Maybe take this function out of here */
  static PhotoGroup* GetFromDirectory(const char* dirname);

  /* Photo group iteration functions */
  void ResetPhotoIterator();
  Photo* GetNextPhoto();

};


#endif
