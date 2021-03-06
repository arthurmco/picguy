#include "JPEGPhoto.hpp"
bool JPEGPhoto::Open() {
  _width = 0;
  _height = 0;
  //Close if it left some fd open
  if (photo_f)
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


/* Close the file and free resources loaded by it*/
void JPEGPhoto::Close()
{
  if (photo_f)
    fclose(photo_f);
  photo_f = nullptr;
}

int JPEGPhoto::GetWidth() {return _width;}
int JPEGPhoto::GetHeight(){return _height;}
int JPEGPhoto::GetBitDepth(){return _bit_depth;}

Pixel* JPEGPhoto::GetRawData(){
  /* If we don't have any pixel data, decompress the image.
     If we do, return what we already have.*/
  if (_data)
    return _data;

    /* For some reason, we need to reopen the file to get raw data again */
    this->Open();

  jpeg_start_decompress(&cinfo);


  _data = new Pixel[_height * _width];
  Pixel* pxPos = new Pixel[ _width];

  int offset = 0;
  while (cinfo.output_scanline < cinfo.image_height){
    /*printf("Getting image (%d - %d) \n",
        cinfo.output_scanline,
        cinfo.image_height); */
    int lines = jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&pxPos, 1);

    memcpy(_data + offset, pxPos, _width * (_bit_depth/8));
    offset += (cinfo.image_width);


  }
  jpeg_finish_decompress(&cinfo);

  delete[] pxPos;
  return _data;

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
