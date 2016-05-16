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

PhotoGroup* PhotoGroup::GetPhotoGroupByIndex(int idx)
{
  return _groups[idx];
}


PhotoGroup* PhotoGroup::GetPhotoGroup(const char* name)
{
  for (auto it = _groups.begin();
       it != _groups.end();
       it++){

    if (!strcmp(name, (*it)->_name.c_str())) {
      return (*it);
    }
  }

  return nullptr;
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

Photo* PhotoGroup::GetPhotoByIndex(int index) {
  return _photos[index];
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

/* Photo group photo iteration functions */
void PhotoGroup::ResetPhotoIterator() {
  it = _photos.begin();
}
Photo* PhotoGroup::GetNextPhoto() {
  if (it == _photos.end()) {
    this->ResetPhotoIterator();
    return nullptr;
  }

  if (_photos.size() == 0) return nullptr;

  return *it++;
}


/* Photo group subgroup iteration functions */
void PhotoGroup::ResetGroupIterator() {
  itg = _groups.begin();
}
PhotoGroup* PhotoGroup::GetNextGroup() {
  if (itg == _groups.end()) {
    this->ResetGroupIterator();
    return nullptr;
  }

  if (_groups.size() == 0) return nullptr;

  return *itg++;
}


/* Get the photo from a directory
   TODO: Maybe take this function out of here */
static PhotoGroup* GetFromDirectory(const char* dirname);
