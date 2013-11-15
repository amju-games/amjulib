#ifndef COMMAND_FIGHT_H_INCLUDED
#define COMMAND_FIGHT_H_INCLUDED

#include <GuiElement.h>
#include "Ve1ObjectChar.h"

namespace Amju
{
class CommandFight : public GuiCommand
{
public:
  CommandFight(Ve1ObjectChar* opp) : m_opponent(opp)
  {
  }

  virtual bool Do();

private:
  Ve1ObjectChar* m_opponent;
};
}

#endif

