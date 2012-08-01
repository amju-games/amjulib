#ifndef VE1_CHARACTER_H
#define VE1_CHARACTER_H

#include "BlinkCharacter.h"
#include "Ve1Object.h"

namespace Amju
{
class Ve1Character : public BlinkCharacter
{
public:
  Ve1Character(Ve1Object* obj) : m_obj(obj) {}

  virtual bool Load(File* f);
  virtual void Draw();

protected:
  RCPtr<Ve1Object> m_obj;
};
}

#endif

