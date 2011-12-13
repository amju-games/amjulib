#ifndef VE1_CHARACTER_H
#define VE1_CHARACTER_H

#include "BlinkCharacter.h"

namespace Amju
{
class Ve1Character : public BlinkCharacter
{
public:
  virtual bool Load(File* f);
};
}

#endif

