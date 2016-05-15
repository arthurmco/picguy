#include "PhotoFormats.hpp"

PhotoFormats::PhotoFormats(){
  _extensions = std::vector<char*>();
  _formats = std::map<int, Photo*>();
}

void PhotoFormats::RegisterFormat(char* extension, Photo* format){
  _extensions.push_back(extension);
  _formats.emplace(_extensions.size()-1, format);
}
void PhotoFormats::UnregisterFormat(char* extension){
  int extindex = 0;
  for (auto it = _extensions.begin(); it != _extensions.end(); it++){
    if (!strcmp(extension, (*it))){
      _formats.erase(extindex);
      return;
    }
    extindex++;
  }

  return;
}

/* Get a format. Return false if it does not exist.
   Return true if it exists and put the format on fmt*/
Photo* PhotoFormats::GetFormat(const char* extension){
  // if (!extension){
  //   extension = "\0";
  // }

  try {
    int extindex = 0;
    for (auto it = _extensions.begin(); it != _extensions.end(); it++){
      if (!strcmp(extension, (*it))){
	return _formats.at(extindex)->Clone();
      }
      extindex++;
    }
    return NULL;
  } catch (std::out_of_range& ex){
    //This block runs only if there's no extension on the list. Return NULL.
    return NULL;
  }
}

/* Retrieve all extensions supported */
std::vector<char*> PhotoFormats::GetFormats() {
  return _extensions;
}
