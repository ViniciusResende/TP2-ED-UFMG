#ifndef LISTCELLH
#define LISTCELLH

#include <string>
#include "memlog.h"
#include "List.hpp"

class ListCell {
  public:  
    ListCell();

    ~ListCell();

  private:
    std::string value;
    ListCell *next;

  friend class List;
};

#endif
