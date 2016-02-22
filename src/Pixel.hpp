/* Pixel.hpp
   Pixel control and conversion routines

   Copyright (C) 2016 - Arthur M 
*/


#ifndef PIXEL_HPP
#define PIXEL_HPP

/* Pixel, in RGB format */
union Pixel {
  unsigned data:24;
  struct {
    char r,g,b;
  } channels_rgb;
  struct {
    char b,g,r;
  } channels_bgr;
};

/* Pixel, in RGBA format */
union PixelAlpha {
  int data;
  struct {
    char r,g,b,a;
  } channels_rgba;
  struct {
    char b,g,r,a;
  } channels_bgra;
};




#endif
