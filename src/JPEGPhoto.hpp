/* JPEGPhoto.hpp
   Class that handles JPEG images

   Copyright (C) 2016 - Arthur M 
*/


#ifndef JPEGPHOTO_HPP
#define JPEGPHOTO_HPP

#include <stdio.h> //FILE*

#include <sys/stat.h> // struct stat
#include <unistd.h>   // and fstat()
#include <fcntl.h>    // open()

#include <stdexcept>

#include "Photo.hpp"

//libjpeg header

extern "C" {
 #include <jpeglib.h> 
}

class JPEGPhoto : public Photo {
private:
  FILE* photo_f = NULL;

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
public:
  JPEGPhoto() : Photo() {};

  /* Copy constructor mandated by PhotoFormats */
  Photo* Clone() override {return new JPEGPhoto;}
  
  /* Opens a file. Returns true on success, false on failure.
     You have to use the method SetName() to set the file name. */
  bool Open() override;
  
  int GetWidth() override;
  int GetHeight() override;
  int GetBitDepth() override;

  /* Returns file size, in bytes */
  long GetSize() override;
  
Pixel* GetRawData();

  ~JPEGPhoto();
};


#endif
