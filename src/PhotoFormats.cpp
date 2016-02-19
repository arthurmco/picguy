#include "PhotoFormats.hpp"

void PhotoFormats::RegisterFormat(char* extension, Photo* format){
  _formats.emplace(extension, *format);
}
void PhotoFormats::UnregisterFormat(char* extension){
  _formats.erase(extension);
}

/* Get a format. Return NULL if it does not exist */
Photo* PhotoFormats::GetFormat(char* extension){
  try {
    return new Photo(_formats.at(extension));
  } catch (std::out_of_range& ex){
    //This block runs only if there's no extension on the list. Return NULL.
    return NULL;
  }
}
