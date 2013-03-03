#ifndef TUTORIAL_NPC_H_INCLUDED
#define TUTORIAL_NPC_H_INCLUDED

#include "Ve1ObjectChar.h"

namespace Amju
{
class Player;

// Set objectives for the player, giving tutorial information, etc.

// TODO Rename this type
// This type of NPC administers the cog tests.
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

private:
  void Trigger();

protected:
  enum CogTestState
  {
    CT_START,
    CT_DO_INTRODUCTION,
    CT_DOING_TESTS,
    CT_DONE
  };

  CogTestState m_ctState;
  bool m_hasTriggered;
  float m_timer;
};
}

#endif

