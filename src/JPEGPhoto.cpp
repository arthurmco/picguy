#include "JPEGPhoto.hpp"
bool JPEGPhoto::Open() {
  //Close if it left some fd open
  if (photo_f != NULL)
    fclose(photo_f);
  
  photo_f = fopen(_name.c_str(), "rb");

  if (!photo_f)
    return false;

  //Set error device
  cinfo.err = jpeg_std_error(&jerr);
  
  //Initializate decompress object.
  jpeg_create_decompress(&cinfo);

  //Read the file
  jpeg_stdio_src(&cinfo, photo_f);

  //Read the header, requires the image
  jpeg_read_header(&cinfo, 1);

  
  /* Don't start the decompression here.
     We'll decompress the image in the 
     GetRawData() function */

  return true;
}
  
int JPEGPhoto::GetWidth() {return cinfo.image_width;}
int JPEGPhoto::GetHeight(){return cinfo.image_height;}
int JPEGPhoto::GetBitDepth(){return cinfo.num_components*8;}

Pixel* JPEGPhoto::GetRawData(){
  
}


JPEGPhoto::~JPEGPhoto(){
  if (photo_f)
    fclose(photo_f);
}
