/* Pixel.hpp
   Pixel control and conversion routines

   Copyright (C) 2016 - Arthur M 
*/


#ifndef PIXEL_HPP
#define PIXEL_HPP

/* Pixel, in RGBA format */
union Pixel {
  int data;
  struct {
    char r,g,b,a;
  } channels;
};

/* These macros operate on Pixel, but returns Pixel::data */
#define BGRA_TO_RGBA(bgra) ((int)bgra.a) | ((int)bgra.b) << 8) | ((int)bgra.g) << 16) | ((int)bgra.r << 34)



#endif
