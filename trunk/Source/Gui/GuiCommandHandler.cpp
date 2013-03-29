/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#include <AmjuFirst.h>
#include "GuiCommandHandler.h"
#include <AmjuFinal.h>

namespace Amju
{
GuiCommandHandler::GuiCommandHandler()
{
  AMJU_CALL_STACK;

  m_it = m_commands.end();
}

void GuiCommandHandler::Undo()
{
  AMJU_CALL_STACK;

  if (!CanUndo())
  {
    return;
  }
  --m_it;
  (*m_it)->Undo();
}

void GuiCommandHandler::Redo()
{
  AMJU_CALL_STACK;

  if (!CanRedo())
  {
    return; 
  }
  (*m_it)->Do();
  ++m_it;
}

bool GuiCommandHandler::CanUndo() const
{
  AMJU_CALL_STACK;

  bool b = (m_it != m_commands.begin() && m_commands.size() > 0);
  return b;
}

bool GuiCommandHandler::CanRedo() const
{
  AMJU_CALL_STACK;

  bool b = (m_it != m_commands.end() && m_commands.size() > 0);
  return b;
}

void GuiCommandHandler::DoNewCommand(PGuiCommand p)
{
  AMJU_CALL_STACK;

  if (!p.GetPtr())
  {
    return;
  }

  if (p->Do())
  {
    // Is undoable, so add to undo list
    // Any undone commands are erased, so cannot be redone.
    m_commands.erase(m_it, m_commands.end());
    m_commands.push_back(p);
    m_it = m_commands.end();
  }
}
}


