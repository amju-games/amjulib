#ifndef TUTORIAL_NPC_H_INCLUDED
#define TUTORIAL_NPC_H_INCLUDED

#include "Ve1ObjectChar.h"
#include "Ve1Character.h"

namespace Amju
{
class TutorialNpc : public Ve1ObjectChar
{
public:
  static const char* TYPENAME;

  virtual const char* GetTypeName() const { return TYPENAME; }
  virtual void Update();
  virtual bool Load(File*);

  virtual void SetMenu(GuiMenu*);

protected:
};
}

#endif

