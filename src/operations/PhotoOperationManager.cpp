#include "PhotoOperationManager.hpp"

bool PhotoOperationManager::Add(const char* alias, PhotoOperation* op) {
    std::string strAlias = std::string{alias};

    if (_operations.find(strAlias) != _operations.end()){
        return false; /* Already added */
    }

    _operations[strAlias] = op;
    return true;
}

bool PhotoOperationManager::Remove(const char* alias) {
    std::string strAlias = std::string{alias};
    if (_operations.find(strAlias) != _operations.end()) {
        delete _operations[strAlias];
        _operations.erase(strAlias);
        return true;
    }

    return false;
}

PhotoOperation* PhotoOperationManager::Get(const char* alias) {
  std::string strAlias = std::string{alias};
  if (_operations.find(strAlias) == _operations.end()) return nullptr;
  return _operations[strAlias];
}
