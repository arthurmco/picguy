/* Pixel.hpp
   Pixel control and conversion routines

   Copyright (C) 2016 - Arthur M
*/


#ifndef PIXEL_HPP
#define PIXEL_HPP

/* Basic structure for pixel */
struct Pixel {
    unsigned char R, G, B;
    Pixel() {
      R = 0;
      G = 0;
      B = 0;
    }


    Pixel(unsigned char r, unsigned char g, unsigned char b) {
      R = r;
      G = g;
      B = b;
    }

    unsigned char GetR() {return R;}
    unsigned char GetG() {return G;}
    unsigned char GetB() {return B;}

} __attribute__((packed));

/* Basic structure for pixels with an alpha-channel */
struct PixelAlpha : public Pixel {
    unsigned char A;
    PixelAlpha(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
      : Pixel(r,g,b)
    {
        A = a;
    }

    PixelAlpha(Pixel& p)
      : Pixel(p.R, p.G, p.B)
    {

    }

    PixelAlpha()
    : Pixel()
    {
      A = 0;
    }

    unsigned char GetR() {return R;}
    unsigned char GetG() {return G;}
    unsigned char GetB() {return B;}
    unsigned char GetA() {return A;}

} __attribute__((packed));;



#endif
