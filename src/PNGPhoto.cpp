#include "PNGPhoto.hpp"

bool PNGPhoto::Open() {
  if (f_PNG)
    fclose(f_PNG);

  f_PNG = fopen(_name.c_str(), "rb");

  if (!f_PNG)
    return false;

  unsigned char header[8];
  fread(&header, 8, 1, f_PNG);
  
  //Verify PNG signature
  if (png_sig_cmp(header, 0, 8)){
    return false;
  }

  _pngstr =
    png_create_read_struct(PNG_LIBPNG_VER_STRING,
			   NULL, NULL, NULL);

  if (!_pngstr)
    return false; //PNG struct creation error.

  _pnginfo = png_create_info_struct(_pngstr);

  if (!_pnginfo){
    png_destroy_read_struct(&_pngstr,
			    (png_infopp)NULL, (png_infopp)NULL);
    return false;
  }

  //Get the pointer to the file
  png_init_io(_pngstr, f_PNG);
  png_set_sig_bytes(_pngstr, 8);

  //Get the file information
  png_read_info(_pngstr, _pnginfo);

  _width = png_get_image_width(_pngstr, _pnginfo);
  _height = png_get_image_height(_pngstr, _pnginfo);
  _bitdepth = png_get_bit_depth(_pngstr, _pnginfo); 

  int colortype = png_get_color_type(_pngstr, _pnginfo);

  switch (colortype){
  case PNG_COLOR_TYPE_PALETTE: //Using a palette. 
  case PNG_COLOR_TYPE_GRAY: //Grayscale channel. 1 color attribute/pixel
    _bitdepth *= 1;
    break;

  case PNG_COLOR_TYPE_GRAY_ALPHA: //Gray+alpha: 2 color attrib/pixel
    _bitdepth *= 2;
    break;

  case PNG_COLOR_TYPE_RGB: //RGB. 3 attrib/pixel
    _bitdepth *= 3;
    break;

  case PNG_COLOR_TYPE_RGB_ALPHA: //RGBA. 4 attrib/pixel
    _bitdepth *= 4;
    break;   
  }

  return true;
}
  
int PNGPhoto::GetWidth() { return _width; }
int PNGPhoto::GetHeight() { return _height; }
int PNGPhoto::GetBitDepth() { return _bitdepth;}

Pixel* PNGPhoto::GetRawData() {
  return NULL;
}
		  
PNGPhoto::~PNGPhoto(){

  if (_pnginfo)
    png_destroy_read_struct(&_pngstr,
			    &_pnginfo,
			    (png_infopp)NULL);

  
  if (f_PNG)
    fclose(f_PNG);
}
