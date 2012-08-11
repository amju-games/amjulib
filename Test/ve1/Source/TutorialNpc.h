#ifndef TUTORIAL_NPC_H_INCLUDED
#define TUTORIAL_NPC_H_INCLUDED

#include "Ve1ObjectChar.h"

namespace Amju
{
class Player;

// Set objectives for the player, giving tutorial information, etc.
class TutorialNpc : public Ve1ObjectChar
{
public:
  static const char* TYPENAME;
  TutorialNpc();

  virtual const char* GetTypeName() const { return TYPENAME; }
  virtual void Update();
  virtual bool Load(File*);
  virtual void SetMenu(GuiMenu*);
  virtual void OnLocationEntry();

  void OnPlayerCollision(Player*);
  void Trigger();

protected:
  bool m_hasDoneCogTests;
  bool m_hasTriggered;
};
}

#endif

