/* PNGPhoto.hpp
   Class that handles PNG images

   Copyright (C) 2016 - Arthur M 
*/


#ifndef PNGPHOTO_HPP
#define PNGPHOTO_HPP

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
  
  /* Copy constructor mandated by PhotoFormats */
  Photo* Clone() override {return new PNGPhoto;}
  
  /* Opens a file. Returns true on success, false on failure.
     You have to use the method SetName() to set the file name. */
  bool Open() override;
  
  int GetWidth() override;
  int GetHeight() override;
  int GetBitDepth() override;

Pixel* GetRawData();

  ~PNGPhoto();


};

#endif
