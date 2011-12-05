#ifndef LOCAL_PLAYER_H_INCLUDED
#define LOCAL_PLAYER_H_INCLUDED

#include "Player.h"

namespace Amju
{
class LocalPlayer : public Player
{
public:
  LocalPlayer();
 
  static const char* TYPENAME;

  virtual const char* GetTypeName() const { return TYPENAME; }
 
};

LocalPlayer* GetLocalPlayer();

}

#endif

