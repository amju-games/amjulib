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

  virtual void SetKeyVal(const std::string& key, const std::string& val);

  // Called when local player adds more fuel to ship. We then send this to server.
  void AddFuel(int delta);

private:

};
}

#endif

