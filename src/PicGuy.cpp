/* PicGuy.cpp
   Principal file

   Copyright (C) 2016 - Arthur M 
*/

#include <stdio.h>
#include <string.h>

#include "PhotoFormats.hpp"

#include "JPEGPhoto.hpp"

int main(int argc, char* argv[]){
  if (argc < 2){
    printf("Test the PhotoFormats class with a file\n");
    printf("Usage: %s [file]\n", argv[0]);
    return 2;
  }

  PhotoFormats f;

  /* Create formats */
  f.RegisterFormat(".jpg", new JPEGPhoto());
  
  char* file = argv[1];
  char* extension = strrchr(file, '.');

  //No extension
  if (!extension)
    extension = "\0";
  
  Photo* format = f.GetFormat(extension);
  
  if (format){
    printf("Format found.\n");
    format->SetName(file);
    if (format->Open()){

      printf("Dimensions: %dx%d, %d bits\n",
	     format->GetWidth(), format->GetHeight(),
	     format->GetBitDepth());
    }
  } else {
    printf("Format not found\n");
  }
  
  
}
