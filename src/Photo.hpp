/* Photo.hpp
   Base class for photo operations

   Copyright (C) 2016 - Arthur M 
*/

#ifndef PHOTO_HPP
#define PHOTO_HPP

#include <string>

#include "Pixel.hpp"

class Photo {
private:
  Pixel* _data;
  std::string _name;

public:
  /* Opens a file. Returns true on success, false on failure.
     You have to use the method SetName() to set the file name. */
  bool Open() {return false; }

  const char* GetName();
  void SetName(const char*);

  char* GetFileExtension() const;

  int GetWidth(){return 0;}
  int GetHeight(){return 0;}
  int GetBitDepth(){return 0;}

  Pixel* GetRawData();
  
};

#endif

