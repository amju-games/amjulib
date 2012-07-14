#ifndef BADDIE_H_INCLUDED
#define BADDIE_H_INCLUDED

#include "Ve1ObjectChar.h"

namespace Amju
{
// NPCs which are obstacles in the game. Players can battle NPCs. If defeated, the Baddie
//  can give a rewards to the player.

class Baddie : public Ve1ObjectChar
{
public:
  static const char* TYPENAME;

  virtual const char* GetTypeName() const;
  virtual void Update();
  virtual bool Load(File*);
};
}

#endif

