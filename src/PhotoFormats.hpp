/* PhotoFormats.hpp
   Class to register and get an appropriate format for your file

   Copyright (C) 2016 - Arthur M
*/

#ifndef PHOTOFORMATS_HPP
#define PHOTOFORMATS_HPP

#include <map>
#include <vector>
#include <stdexcept> //out_of_range exception

#include <cstring>

#include "Photo.hpp"

class PhotoFormats {
private:

  /* The integer is a index on the extension list */
  std::map<int/*extension*/, Photo* /*format*/> _formats;

  /* The extension list */
  std::vector<char*> _extensions;

public:
  PhotoFormats();

  void RegisterFormat(char* extension, Photo* format);
  void UnregisterFormat(char* extension);

  /* Get a format. Return NULL if it does not exist */
  Photo* GetFormat(const char* extension);


};


#endif
