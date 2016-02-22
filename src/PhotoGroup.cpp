#include "PhotoGroup.hpp"

/* Adds a photo, returns an ID number */
int PhotoGroup::AddPhoto(Photo* p){
  this->_photos.push_back(p);
  return p->GetID();
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


/* Get the photo from a directory
   TODO: Maybe take this function out of here */
static PhotoGroup* GetFromDirectory(const char* dirname);
 
