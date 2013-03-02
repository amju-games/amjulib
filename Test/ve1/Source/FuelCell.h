#ifndef FUELCELL_H_INCLUDED
#define FUELCELL_H_INCLUDED

#include "Furniture.h"

namespace Amju 
{
class FuelCellManager : public Ve1Object
{
public:
  static const char* TYPENAME;
  virtual const char* GetTypeName() const;

  // Create fuel cells in new location, local to client only
  virtual void OnLocationEntry();

  virtual void SetEditMenu(GuiMenu*) {}

  virtual void Update();

};

class FuelCell : public Furniture
{
public:
  static const char* TYPENAME;
  virtual const char* GetTypeName() const;

  FuelCell();

  virtual bool Load(File*);
  virtual void SetEditMenu(GuiMenu*) {}
  virtual void SetMenu(GuiMenu*) {}

  void SetOwner(Ve1Object* v) { m_owner = v; }

private:
  Ve1Object* m_owner;
};
} // namespace
#endif
