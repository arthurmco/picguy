/*  Converts image to black and white

    Copyright (C) 2016 - Arthur M
*/

#include "PhotoOperation.hpp"


#ifndef BLACKNWHITEOPERATION_HPP
#define BLACKNWHITEOPERATION_HPP

class BlackAndWhiteOperation : public PhotoOperation
{
public:
  const char* GetName() const override {return "Black and White";}

  /*  Show the window to setup the operation */
  bool ShowWindow() override {return true; };

  /* Do the operation. Returns true on success, false on failure.
     The photo is modified on the process, so if you want to preserve
     the original, you should copy it.
    */
  bool DoOperation(Photo* ph) override;
};

#endif /* end of include guard: BLACKNWHITEOPERATION_HPP */
