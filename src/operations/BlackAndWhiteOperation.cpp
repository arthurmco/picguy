#include "BlackAndWhiteOperation.hpp"

bool BlackAndWhiteOperation::DoOperation(Photo* ph) {
  unsigned int area = ph->GetWidth() * ph->GetHeight();
  if (ph->GetBitDepth() >= 32) {
    PixelAlpha* p = (PixelAlpha*) ph->GetRawData();

    for (unsigned i = 0; i < area; i++) {
        unsigned media = (p[i].R + p[i].G + p[i].B) / 3;
        p[i].R = media & 0xff;
        p[i].G = media & 0xff;
        p[i].B = media & 0xff;
    }

  } else {
    Pixel* p = ph->GetRawData();


    for (unsigned i = 0; i < area; i++) {
        unsigned media = (p[i].R + p[i].G + p[i].B) / 3;
        p[i].R = media & 0xff;
        p[i].G = media & 0xff;
        p[i].B = media & 0xff;
    }
  }

  return true;
};
