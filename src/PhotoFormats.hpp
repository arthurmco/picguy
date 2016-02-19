/* PhotoFormats.hpp
   Class to register and get an appropriate format for your file

   Copyright (C) 2016 - Arthur M 
*/

#ifndef PHOTOFORMATS_HPP
#define PHOTOFORMATS_HPP

#include <map>
#include <stdexcept> //out_of_range exception

#include "Photo.hpp"

class PhotoFormats {
private:
  std::map<char*/*extension*/,Photo/*format*/> _formats;

public:
void RegisterFormat(char* extension, Photo* format);
void UnregisterFormat(char* extension);

/* Get a format. Return NULL if it does not exist */
Photo* GetFormat(char* extension);

  
};


#endif



