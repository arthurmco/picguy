#include "PhotoGroup.hpp"

PhotoGroup::PhotoGroup(const char* name, int id){
  _name = std::string(name);
  _id = id;
  _parent = nullptr;
}

/* Adds a photo, returns an ID number */
int PhotoGroup::AddPhoto(Photo* p){
  this->_photos.push_back(p);
  return p->GetID();
}


/* Adds a group */
int PhotoGroup::AddPhotoGroup(PhotoGroup* pg){
  this->_groups.push_back(pg);
  pg->_parent = this;
  return 1;
}

/* Removes the photo whose ID is 'id' */
void PhotoGroup::RemovePhoto(int id){
  for (auto it = _photos.begin();
       it != _photos.end();
       it++){

    if ((*it)->GetID() == id) {
      this->_photos.erase(it);
      return;
    }
  }

}

/* Get the photo count */
int PhotoGroup::GetPhotoCount(){
  return _photos.size();
}

Photo* PhotoGroup::GetPhoto(int id){
  for (auto it = _photos.begin();
       it != _photos.end();
       it++){

    if ((*it)->GetID() == id) {
      return (Photo*)(*it);
    }
  }

}

void PhotoGroup::SetName(const char* name){
  _name = std::string(name);
}
char* PhotoGroup::GetName() const {
  return const_cast<char*>(_name.c_str());
}

  /* Get the parent */
PhotoGroup* PhotoGroup::GetParent() const { return _parent; }

/* Get the ID */
int PhotoGroup::GetID() const { return _id; }


/* Get the photo from a directory
   TODO: Maybe take this function out of here */
static PhotoGroup* GetFromDirectory(const char* dirname);
