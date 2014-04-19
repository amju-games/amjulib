#ifndef HOT_LAVAL_H_INCLUDED
#define HOT_LAVAL_H_INCLUDED

#include "Ve1Object.h"

namespace Amju
{
class HotLava : public Ve1Object
{
public:
  HotLava();

  static const char* TYPENAME;
  virtual const char* GetTypeName() const;

  virtual void SetEditMenu(GuiMenu*);
  //virtual void Update();
};
}

#endif
