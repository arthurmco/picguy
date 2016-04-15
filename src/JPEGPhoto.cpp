#include "JPEGPhoto.hpp"
bool JPEGPhoto::Open() {
  //Close if it left some fd open
  if (photo_f != NULL)
    fclose(photo_f);

  //Check image size.


  photo_f = fopen(_name.c_str(), "rb");

  if (!photo_f)
    return false;

  //Set error device
  cinfo.err = jpeg_std_error(&jerr);

  jerr.error_exit =
    [](j_common_ptr errinfo)
    {throw std::runtime_error("Malformed JPEG file");};

  try {
    //Initializate decompress object.
    jpeg_create_decompress(&cinfo);

    //Read the file
    jpeg_stdio_src(&cinfo, photo_f);

    //Read the header, requires the image
    if (jpeg_read_header(&cinfo, 1) != JPEG_HEADER_OK){
      return false; //Error happened.
    }

  } catch (std::runtime_error&) {
    /* JPEG errors happen when data is invalid.
       Set the most likely error code and return false */
    errno = ENODATA;
    return false;
  }


  _width = cinfo.image_width;
  _height = cinfo.image_height;
  _bit_depth = cinfo.num_components*8;

  /* Don't start the decompression here.
     We'll decompress the image in the
     GetRawData() function */

  return true;
}

int JPEGPhoto::GetWidth() {return _width;}
int JPEGPhoto::GetHeight(){return _height;}
int JPEGPhoto::GetBitDepth(){return _bit_depth;}

Pixel* JPEGPhoto::GetRawData(){
  /* If we don't have any pixel data, decompress the image.
     If we do, return what we already have.*/
  if (_pixel)
    return _pixel;

  jpeg_start_decompress(&cinfo);


  _pixel = new Pixel[_height * _width * (_bit_depth/8)];
  Pixel* pxPos = new Pixel[ _width * (_bit_depth/8)];

  int offset = 0;
  while (cinfo.output_scanline < cinfo.image_height){
    printf("Getting image (%d - %d) \n",
        cinfo.output_scanline,
        cinfo.image_height);
    int lines = jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&pxPos, 1);

    memcpy(_pixel + offset, pxPos, _width * (_bit_depth/8));
    offset += (cinfo.image_width * cinfo.num_components);


  }
  
  jpeg_finish_decompress(&cinfo);
  return _pixel;

}


/* Returns file size, in bytes */
long JPEGPhoto::GetSize(){
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

JPEGPhoto::~JPEGPhoto(){
  if (photo_f)
    fclose(photo_f);
}
