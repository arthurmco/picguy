/* JPEGPhoto.hpp
   Class that handles JPEG images

   Copyright (C) 2016 - Arthur M 
*/


#ifndef JPEGPHOTO_HPP
#define JPEGPHOTO_HPP

#include "Photo.hpp"

class JPEGPhoto : public Photo {

public:
  /* Opens a file. Returns true on success, false on failure.
     You have to use the method SetName() to set the file name. */
  bool Open();

  const char* GetFileExtension() {return ".jpg";}
  
  int GetWidth();
  int GetHeight();
  int GetBitDepth();

  
};


#endif
