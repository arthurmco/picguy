#include "Photo.hpp"

static int lastid = 0;

Photo::Photo(){
  /* Get an ID */;
  _id = lastid++;
  _data = nullptr;
}

const char* Photo::GetFileExtension(){
  if (_name != ""){
    return strrchr(_name.c_str(), '.');
  }
}

const char* Photo::GetName() {
  return _name.c_str();

}
void Photo::SetName(const char* name){
  _name = std::string(name);
}

int Photo::GetID(){return _id;}


/* Frees the memory occupied by the pixel raw data */
void Photo::FreeRawData() {
  if (_data) {
    delete[] _data;
  }

  _data = nullptr;
}

/* Gets a 128x128 thumbnail for use in icons, same bitdepth as image */
Pixel* Photo::GetThumbnail()
{
    if (!_data) {
        _data = this->GetRawData();
    }

    if (!_thumbnail) {

        /*  Determine the biggest possible image size that can fit in a
            128x128 square, the default thumbnail size that this class
            can generate.  */
        if (GetWidth() > 128 && GetHeight() > 128) {
            double wratio = GetWidth()/128.0;
            double hratio = GetHeight()/128.0;

            if (wratio > hratio) {
                /* Wide photos */
                _thumbW = 128;
                _thumbH = 128*(hratio/wratio);
            } else if ( hratio > wratio ) {
                /* Tall (or cellphone-y?) photos */
                _thumbH = 128;
                _thumbW = 128*(wratio/hratio);
            } else {
                /* Square photos */
                _thumbH = _thumbW = 128;
            }

        } else {
            /* These images are smaller than 128x128, use their sizes */
            _thumbH = GetHeight();
            _thumbW = GetWidth();
        }

        _thumbnail = new PixelAlpha[_thumbH*_thumbW];

        double multiX = this->GetWidth()/(_thumbW*1.0);
        double multiY = this->GetHeight()/(_thumbH*1.0);
        int w = this->GetWidth();

        for (int y = 0; y < _thumbH; y++) {
            for (int x = 0; x < _thumbW; x++) {
                double r=0.0,g=0.0,b=0.0,a=0.0;
                double lr=0.0,lg=0.0,lb=0.0,la=0.0;

                int colors = 0;

                /*  Choose some lucky pixels that will be part of our thumbnail
                    multi?/4 seems to be a sufficient pixel-choosing value */
                for (int yA = 0; yA < multiY; yA += fmax(1,multiY/4)) {
                    for (int xA = 0; xA < multiX; xA += fmax(1,multiX/4)) {
                        int dataX = (x*multiX)+xA;
                        int dataY = (y*multiY)+yA;

                        if (this->GetBitDepth() >= 32) {
                            PixelAlpha* data32 = (PixelAlpha*)_data;

                            /*  Average the pixels. This can be wrong, but
                                works now. */

                            r += data32[dataY*w+dataX].R;
                            g += data32[dataY*w+dataX].G;
                            b += data32[dataY*w+dataX].B;
                            a += data32[dataY*w+dataX].A;

                        } else {
                            r += _data[dataY*w+dataX].R;
                            g += _data[dataY*w+dataX].G;
                            b += _data[dataY*w+dataX].B;
                            a += 0xff;
                        }

                        lr=r;   lg=g;   lb=b;   la=a;

                        colors++;
                    }
                }
                r /= colors;
                g /= colors;
                b /= colors;
                a /= colors;

                /* Get the average, fix overflows */
                _thumbnail[(y*_thumbW)+x].R = (unsigned char)fmin(r,0xff);
                _thumbnail[(y*_thumbW)+x].G = (unsigned char)fmin(g,0xff);
                _thumbnail[(y*_thumbW)+x].B = (unsigned char)fmin(b,0xff);
                _thumbnail[(y*_thumbW)+x].A = 0xff;

            }
        }




    }

    return (Pixel*)_thumbnail;
}

int Photo::GetThumbnailWidth() { return _thumbW;}
int Photo::GetThumbnailHeight() { return _thumbH;}
