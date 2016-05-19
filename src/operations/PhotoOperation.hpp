/* Photo operation

   Copyright (C) 2016 - Arthur M
*/

#include "../Photo.hpp"

#ifndef PHOTOOPERATION_HPP
#define PHOTOOPERATION_HPP

/**
  A photo operation is something you can do with the photo, like a filter, or
  a simple operation.
**/
class PhotoOperation {
public:
  virtual const char* GetName() const {return "Unknown";}

  /* Do the operation. Returns true on success, false on failure.
     The photo is modified on the process, so if you want to preserve
     the original, you should copy it.
    */
  virtual bool DoOperation(Photo* ph);

};



#endif /* end of include guard: PHOTOOPERATION_HPP */
