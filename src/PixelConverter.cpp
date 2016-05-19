#include "PixelConverter.hpp"

Pixel* PixelConverter::RGBAtoRGB(PixelAlpha* pixel, size_t area) {
  Pixel* px = new Pixel[area];
  for (size_t i = 0; i < area; i++) {
    px[i].R = pixel[i].R;
    px[i].G = pixel[i].G;
    px[i].B = pixel[i].B;
  }


}
PixelAlpha* PixelConverter::RGBtoRGBA(Pixel* pixel, size_t area) {
  PixelAlpha* px = new PixelAlpha[area];
  for (size_t i = 0; i < area; i++) {
    px[i].R = pixel[i].R;
    px[i].G = pixel[i].G;
    px[i].B = pixel[i].B;
    px[i].A = 0xff;
  }



}
