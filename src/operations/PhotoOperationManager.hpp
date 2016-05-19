/* Manages all photo operations.
   Allows you to add, remove and get any operation

   Copyright (C) 2016 - Arthur M
*/

#include <map>
#include <string>

#include "PhotoOperation.hpp"

#ifndef PHOTOOPERATIONMANAGER_HPP
#define PHOTOOPERATIONMANAGER_HPP

class PhotoOperationManager {
private:
  std::map<std::string /*alias*/, PhotoOperation*> _operations;


public:
  /* Adds an operation. Returns false if already exists */
  bool Add(const char* alias, PhotoOperation* op);

  /* Remove an operation. Returns false if doesn't exist */
  bool Remove(const char* alias);

  /* Get an operation. Returns null if doesn't exist */
  PhotoOperation* Get(const char* alias);

};



#endif /* end of include guard: PHOTOOPERATIONMANAGER_HPP */
