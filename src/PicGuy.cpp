/* PicGuy.cpp
   Principal file

   Copyright (C) 2016 - Arthur M 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PhotoFormats.hpp"
#include "PhotoGroup.hpp"

#include "JPEGPhoto.hpp"
#include "PNGPhoto.hpp"

int main(int argc, char* argv[]){
  if (argc < 2){
    printf("Test the PhotoFormats class with a file\n");
    printf("Usage: %s [file]\n", argv[0]);
    return 2;
  }

  PhotoFormats f;

  /* Register supported formats */
  f.RegisterFormat(".jpg", new JPEGPhoto());
  f.RegisterFormat(".jpeg", new JPEGPhoto());
  f.RegisterFormat(".png", new PNGPhoto());
  
  /* Test the PhotoGroup class */
  PhotoGroup photogrp;
  for (int i = 1; i < argc; i++){

      char* file = argv[i];
      char* extension = strrchr(file, '.');

      printf("\n File: %s\n", file);
      
      //No extension
      if (!extension)
	extension = "\0";

    
      Photo* format = f.GetFormat(extension);
      
      if (format){
	format->SetName(file);
	if (format->Open()){
 	  
	  printf("\tDimensions: %dx%d, %d bits\n",
		 format->GetWidth(), format->GetHeight(),
		 format->GetBitDepth());

	  photogrp.AddPhoto(format);
	} else {
	  char err[128];
	  memset(err, '\0', 128);

	  printf("Error: ");
	  perror(err);
	  printf("\n");
	
	}
      } else {
	printf("Format not found\n");
      }
      
  }

  printf("\n\t %d photos added\n", photogrp.GetPhotoCount());
  
  
}
