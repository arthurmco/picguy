#include "Photo.hpp"

static int lastid = 0;

Photo::Photo(){
  /* Get an ID */;
  _id = lastid++;
}

const char* Photo::GetFileExtension(){
  if (_name != ""){
    return strrchr(_name.c_str(), '.');
  }
}

const char* Photo::GetName() {
  return _name.c_str();

}
void Photo::SetName(const char* name){
  _name = std::string(name);
}


int Photo::GetID(){return _id;}
