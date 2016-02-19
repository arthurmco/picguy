/* PicGuy.cpp
   Principal file

   Copyright (C) 2016 - Arthur M 
*/

#include <stdio.h>
#include <string.h>

#include "PhotoFormats.hpp"

int main(int argc, char* argv[]){
  if (argc < 2){
    printf("Test the PhotoFormats class with a file\n");
    printf("Usage: %s [file]\n", argv[0]);
    return 2;
  }

  PhotoFormats f;
  char* file = argv[1];
  char* extension = strrchr(file, '.');

  Photo* format = f.GetFormat(extension);

  if (format){
    printf("Format found.\n");
  } else {
    printf("Format not found\n");
  }
  
  
}
