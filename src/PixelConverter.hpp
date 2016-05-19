/* Helper class to convert between pixel formats.

   Copyright (C) 2016 - Arthur M
*/

#ifndef PIXELCONVERTER_HPP
#define PIXELCONVERTER_HPP

#include <cstddef> //size_t
#include "Pixel.hpp"

class PixelConverter {
public:

  /* Converts between pixel formats
      area = width * height, obviously

      You have to clean the old Pixels
  */
  static Pixel* RGBAtoRGB(PixelAlpha* old, size_t area);
  static PixelAlpha* RGBtoRGBA(Pixel* old, size_t area);

};

#endif /* end of include guard: PIXELCONVERTER_HPP */
