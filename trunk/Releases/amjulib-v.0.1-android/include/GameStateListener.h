#pragma once

#include "EventListener.h"

namespace Amju
{
class GameState;

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

  virtual bool OnKeyEvent(const KeyEvent& e); 
  virtual bool OnCursorEvent(const CursorEvent& e);
  virtual bool OnRotationEvent(const RotationEvent& e); 
  virtual bool OnJoyAxisEvent(const JoyAxisEvent& e);
  virtual bool OnButtonEvent(const ButtonEvent& e);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent& e); 
  virtual bool OnDoubleClickEvent(const DoubleClickEvent& e); 
  virtual bool OnBalanceBoardEvent(const BalanceBoardEvent& e);
  virtual bool OnTextEvent(const TextEvent& e);
  virtual bool OnQuitEvent();
  virtual bool OnResizeEvent(const ResizeEvent& e);

private:
  // The game state that this object forwards events to.
  GameState* m_gs;
};

}

