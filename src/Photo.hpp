/* Photo.hpp
   Base class for photo operations

   Copyright (C) 2016 - Arthur M
*/

#ifndef PHOTO_HPP
#define PHOTO_HPP

#include <string>
#include <cstring>
#include <cmath>

#include "Pixel.hpp"

class Photo {
protected:
    /*  Default pixel data container. This class expects you put final
        raw pixel content in this variable */
    Pixel* _data = nullptr;

    /* Thumbnail data container */
    PixelAlpha* _thumbnail = nullptr;
    int _thumbW, _thumbH;

    std::string _name;
    int _id;


public:
  Photo();

  virtual Photo* Clone(){return new Photo;}

  /* Opens a file. Returns true on success, false on failure.
     You have to use the method SetName() to set the file name. */
  virtual bool Open() {return false; }

  /* Close the file and free resources loaded by it*/
  virtual void Close(){return ;}

  const char* GetName();
  void SetName(const char*);

  const char* GetFileExtension();

  virtual int GetWidth(){return 0;}
  virtual int GetHeight(){return 0;}
  virtual int GetBitDepth(){return 0;}
  virtual const char* GetType(){return NULL;}

  /* Returns file size, in bytes */
  virtual long GetSize(){return 0;}

  int GetID();

  virtual Pixel* GetRawData(){return NULL;}

  /* Frees the memory occupied by the pixel raw data */
  void FreeRawData();

  /* Gets a 128x128 thumbnail for use in icons */
  Pixel* GetThumbnail();
  int GetThumbnailWidth();  int GetThumbnailHeight();
};

#endif
