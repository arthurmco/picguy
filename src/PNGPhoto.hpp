
/* PNGPhoto.hpp
   Class that handles PNG images

   Copyright (C) 2016 - Arthur M
*/


#ifndef PNGPHOTO_HPP
#define PNGPHOTO_HPP

#include <sys/stat.h> // struct stat
#include <unistd.h>   // and fstat()
#include <fcntl.h>    // open()

#include "Photo.hpp"

extern "C" {
#include <png.h>
}

class PNGPhoto : public Photo {
private:
  FILE* f_PNG = NULL;

  /* PNG library structures */
  png_infop _pnginfo;
  png_structp _pngstr;

  int _width = 0, _height = 0, _bitdepth = 0;
public:

  PNGPhoto() : Photo() {};

  /* Copy constructor mandated by PhotoFormats */
  Photo* Clone() override {return new PNGPhoto;}

  /* Opens a file. Returns true on success, false on failure.
     You have to use the method SetName() to set the file name. */
  bool Open() override;

  /* Close the file and free resources loaded by it*/
  void Close() override;

  int GetWidth() override;
  int GetHeight() override;
  int GetBitDepth() override;
  const char* GetType() override { return "PNG"; }
  long GetSize() override;

Pixel* GetRawData();

  ~PNGPhoto();


};

#endif
