#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include "Ve1Object.h"

namespace Amju
{
class Room : public Ve1Object
{
public:
  virtual void Update();
  virtual bool Load(File*);

  static const char* TYPENAME;
  virtual const char* GetTypeName() const;
  virtual void SetEditMenu(GuiMenu*) {} // TODO chop this 

  virtual void OnLocationEntry();

};
}

#endif
