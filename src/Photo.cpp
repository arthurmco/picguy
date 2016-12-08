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
        _thumbnail = new PixelAlpha[128*128];

        double multiX = this->GetWidth()/128.0;
        double multiY = this->GetHeight()/128.0;
        int w = this->GetWidth();

        for (int y = 0; y < 128; y++) {
            for (int x = 0; x < 128; x++) {
                int r=0,g=0,b=0,a=0;

                for (int yA = 0; yA < multiY; yA++) {
                    for (int xA = 0; xA < multiX; xA++) {
                        int dataX = (x*multiX)+xA;
                        int dataY = (y*multiY)+yA;

                        if (this->GetBitDepth() >= 32) {
                            PixelAlpha* data32 = (PixelAlpha*)_data;
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
                    }
                }
                r /= (multiX*multiY);
                g /= (multiX*multiY);
                b /= (multiX*multiY);
                a /= (multiX*multiY);

                _thumbnail[(y*128)+x].R = (unsigned char)(r&0xff);
                _thumbnail[(y*128)+x].G = (unsigned char)(g&0xff);
                _thumbnail[(y*128)+x].B = (unsigned char)(b&0xff);
                _thumbnail[(y*128)+x].A = 0xff;

            }
        }




    }

    return (Pixel*)_thumbnail;
}
