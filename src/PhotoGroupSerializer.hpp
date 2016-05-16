/* PhotoGroup.hpp
   Saves group info into a file, in a specific format.

   Copyright (C) 2016 - Arthur M
*/
#include <cstdio>
#include "PhotoGroup.hpp"

#ifndef PHOTOGROUPSERIALIZER_HPP
#define PHOTOGROUPSERIALIZER_HPP


class PhotoGroupSerializer {
public:
  /* Save the 'gp' photogroup and its childs */
  static bool Save(PhotoGroup* gp, const char* filename);


};




#endif /* end of include guard: PHOTOGROUPSERIALIZER_HPP */
