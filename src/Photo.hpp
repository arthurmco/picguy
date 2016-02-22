/* Photo.hpp
   Base class for photo operations

   Copyright (C) 2016 - Arthur M 
*/

#ifndef PHOTO_HPP
#define PHOTO_HPP

#include <string>
#include <cstring>

#include "Pixel.hpp"

class Photo {
protected:
  Pixel* _data;
  std::string _name;

public:
  Photo(){}

  virtual Photo* Clone(){return new Photo;}
  
  /* Opens a file. Returns true on success, false on failure.
     You have to use the method SetName() to set the file name. */
  virtual bool Open() {return false; }

  const char* GetName();
  void SetName(const char*);

  const char* GetFileExtension();

  virtual int GetWidth(){return 0;}
  virtual int GetHeight(){return 0;}
  virtual int GetBitDepth(){return 0;}

  Pixel* GetRawData();
  
};

#endif

