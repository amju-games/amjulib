#include <GameObjectFactory.h>
#include "FuelCell.h"

namespace Amju
{
const char* FuelCell::TYPENAME = "fuelcell";

GameObject* CreateFuelCell()
{
  return new FuelCell;
}

static bool registered = TheGameObjectFactory::Instance()->Add(FuelCell::TYPENAME, CreateFuelCell);

const char* FuelCell::GetTypeName() const
{
  return TYPENAME;
}

FuelCell::FuelCell()
{
}
} // namespace
