/* PhotoGroup.hpp
   Saves group info into a file, in a specific format.

   Copyright (C) 2016 - Arthur M
*/
#include <cstdio>
#include <cctype> //isspace()
#include <cstring>
#include "PhotoGroup.hpp"
#include "PhotoFormats.hpp"

#ifndef PHOTOGROUPSERIALIZER_HPP
#define PHOTOGROUPSERIALIZER_HPP


class PhotoGroupSerializer {
private:
  static PhotoFormats* pfmt;

public:
  /* Save the 'gp' photogroup and its childs */
  static bool Save(PhotoGroup* gp, const char* filename);

  /* Open the group at filename 'file' and save it on 'gp' */
  static bool Open(const char* filename, PhotoGroup* gp);

  static void SetFormatData(PhotoFormats* fmt);

};




#endif /* end of include guard: PHOTOGROUPSERIALIZER_HPP */
