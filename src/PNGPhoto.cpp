#include "PNGPhoto.hpp"

bool PNGPhoto::Open() {

  _width = 0;
  _height = 0;

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

  /* Prepare jmpbuf */
  if (setjmp(png_jmpbuf(_pngstr)))
     return false;

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

/* Close the file and free resources loaded by it*/
void PNGPhoto::Close()
{
  if (_pnginfo)
    png_destroy_read_struct(&_pngstr,
          &_pnginfo,
          (png_infopp)NULL);

  fclose(f_PNG);
  f_PNG = nullptr;
}

int PNGPhoto::GetWidth() { return _width; }
int PNGPhoto::GetHeight() { return _height; }
int PNGPhoto::GetBitDepth() { return _bitdepth;}

Pixel* PNGPhoto::GetRawData() {
  if (_width == 0 || _height == 0) return nullptr;

  if (_data) return _data;

  if (!f_PNG) {
      if (!this->Open()) return nullptr;
  }

  /* Prepare jmpbuf */
  if (setjmp(png_jmpbuf(_pngstr)))
      return nullptr;

  /* Get the image in PNG way */
  png_bytep* row_ptrs = (png_bytep*) malloc(sizeof(png_bytep) * _height);

  for (int i = 0; i < _height; i++)
    row_ptrs[i] = (png_byte*) malloc(png_get_rowbytes(_pngstr, _pnginfo));

  png_read_image(_pngstr, row_ptrs);

  /* Convert it to Pixel way */
  int stride = _bitdepth/8;

  /* TODO: Extract the full 32-bit colors and convert later */
  _bitdepth = 24;

  _data = new Pixel[_width * _height];
  char** rows = (char**)row_ptrs;
  for (int y = 0; y < _height; y++) {
       for (int x = 0; x < _width; x++) {
        _data[y*_width+x].R = rows[y][x*stride+0];
        _data[y*_width+x].G = rows[y][x*stride+1];
        _data[y*_width+x].B = rows[y][x*stride+2];

      }

  }

  return _data;

}

/* Returns file size, in bytes */
long PNGPhoto::GetSize(){
  /* Opens a file descriptor, fstat()it, get the size and close */
  int fd = open(_name.c_str(), O_RDONLY);

  if (fd < 0)
    return 0; //Error opening the file.

  struct stat st;
  fstat(fd, &st);

  long size = st.st_size;

  close(fd);

  return size;
}


PNGPhoto::~PNGPhoto(){

  if (_pnginfo)
    png_destroy_read_struct(&_pngstr,
			    &_pnginfo,
			    (png_infopp)NULL);


  if (f_PNG)
    fclose(f_PNG);
}
