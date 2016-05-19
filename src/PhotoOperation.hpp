/* Photo operation

   Copyright (C) 2016 - Arthur M
*/

#include "Photo.hpp"

#ifndef PHOTOOPERATION_HPP
#define PHOTOOPERATION_HPP

class PhotoOperation {
  virtual const char* GetName(); const; {return "Unknown";}

  /* Do any operation. Returns true on success */
  virtual bool DoOperation(Pixel* pixels, unsigned w, unsigned h)=0;
  virtual bool DoOperation(PixelAlpha* pixels, unsigned w, unsigned h)=0;
};



#endif /* end of include guard: PHOTOOPERATION_HPP */
