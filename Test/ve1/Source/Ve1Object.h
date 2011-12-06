#ifndef VE1_OBJECT_H
#define VE1_OBJECT_H

#include <GameObject.h>

namespace Amju
{
class Ve1Object : public GameObject
{
public:
  // Objects update their own state
  virtual bool UpdateState(PXml xml) {}
};
}

#endif

