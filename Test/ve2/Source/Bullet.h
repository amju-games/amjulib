#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "Ve1Object.h"

namespace Amju
{
class Bullet : public Ve1Object
{
public:
  static const char* TYPENAME;

  Bullet();
  virtual const char* GetTypeName() const;

  virtual void Update();

  // Call if bullet hits something or goes off world
  void Destroy();
};
}

#endif
