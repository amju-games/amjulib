#ifndef FUELCELL_H_INCLUDED
#define FUELCELL_H_INCLUDED

#include "Furniture.h"

namespace Amju 
{
class FuelCell : public Furniture
{
public:
  static const char* TYPENAME;
  virtual const char* GetTypeName() const;

  FuelCell();

};
} // namespace
#endif
