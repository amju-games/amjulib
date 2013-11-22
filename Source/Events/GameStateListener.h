#pragma once

#include <GameState.h>
#include "EventListener.h"

namespace Amju
{
// Class which forwards events on to the current Game State.
// Can't put the game state directly into the map of listeners because
// game states are singletons.
// Create a new one each time a new game state becomes active. 
// Remove the old listener when a game state deactivates!
// This is done in GameState::Activate() and Deactivate(), so these base class
//  impls should be called from subclass impls.

class GameStateListener : public EventListener
{
public:
  GameStateListener(GameState* gs) : m_gs(gs) {}

  virtual bool OnKeyEvent(const KeyEvent& e) 
  { 
    return m_gs->OnKeyEvent(e); 
  }

  virtual bool OnCursorEvent(const CursorEvent& e) 
  { 
    return m_gs->OnCursorEvent(e); 
  }

  virtual bool OnRotationEvent(const RotationEvent& e) 
  { 
    return m_gs->OnRotationEvent(e); 
  }

  virtual bool OnJoyAxisEvent(const JoyAxisEvent& e) 
  { 
    return m_gs->OnJoyAxisEvent(e);  
  }

  virtual bool OnButtonEvent(const ButtonEvent& e) 
  { 
    return m_gs->OnButtonEvent(e); 
  }

  virtual bool OnMouseButtonEvent(const MouseButtonEvent& e) 
  { 
    return m_gs->OnMouseButtonEvent(e); 
  }

  virtual bool OnDoubleClickEvent(const DoubleClickEvent& e) 
  { 
    return m_gs->OnDoubleClickEvent(e); 
  }

  virtual bool OnBalanceBoardEvent(const BalanceBoardEvent& e) 
  { 
    return m_gs->OnBalanceBoardEvent(e); 
  }

  virtual bool OnTextEvent(const TextEvent& e) 
  { 
    return m_gs->OnTextEvent(e); 
  }

  virtual bool OnQuitEvent() 
  { 
    return m_gs->OnQuitEvent(); 
  }

  virtual bool OnResizeEvent(const ResizeEvent& e) 
  { 
    return m_gs->OnResizeEvent(e); 
  }

private:
  // The game state that this object forwards events to.
  GameState* m_gs;
};

}

