#include "PhotoFormats.hpp"

PhotoFormats::PhotoFormats(){
  _extensions = std::vector<char*>();
}

void PhotoFormats::RegisterFormat(char* extension, Photo* format){
  _extensions.push_back(extension);
  _formats.emplace(_extensions.size()-1, *format);
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

/* Get a format. Return NULL if it does not exist */
Photo* PhotoFormats::GetFormat(char* extension){
  try {
    int extindex = 0;
    for (auto it = _extensions.begin(); it != _extensions.end(); it++){
      if (!strcmp(extension, (*it))){
	  return new Photo(_formats.at(extindex));
      }
      extindex++;
    }
    return NULL;
  } catch (std::out_of_range& ex){
    //This block runs only if there's no extension on the list. Return NULL.
    return NULL;
  }
}
