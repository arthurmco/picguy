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

#endif
