#ifndef SPACESHIP_H_INCLUDED
#define SPACESHIP_H_INCLUDED

#include "Building.h"

namespace Amju
{
class Spaceship : public Building
{
public:
  static const char* TYPENAME;
  virtual const char* GetTypeName() const;

  Spaceship();

};
}

#endif

