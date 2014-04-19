#include <GameState.h>
#include "GameStateListener.h"

namespace Amju
{
  bool GameStateListener::OnKeyEvent(const KeyEvent& e) 
  { 
    return m_gs->OnKeyEvent(e); 
  }

  bool GameStateListener::OnCursorEvent(const CursorEvent& e) 
  { 
    return m_gs->OnCursorEvent(e); 
  }

  bool GameStateListener::OnRotationEvent(const RotationEvent& e) 
  { 
    return m_gs->OnRotationEvent(e); 
  }

  bool GameStateListener::OnJoyAxisEvent(const JoyAxisEvent& e) 
  { 
    return m_gs->OnJoyAxisEvent(e);  
  }

  bool GameStateListener::OnButtonEvent(const ButtonEvent& e) 
  { 
    return m_gs->OnButtonEvent(e); 
  }

  bool GameStateListener::OnMouseButtonEvent(const MouseButtonEvent& e) 
  { 
    return m_gs->OnMouseButtonEvent(e); 
  }

  bool GameStateListener::OnDoubleClickEvent(const DoubleClickEvent& e) 
  { 
    return m_gs->OnDoubleClickEvent(e); 
  }

  bool GameStateListener::OnBalanceBoardEvent(const BalanceBoardEvent& e) 
  { 
    return m_gs->OnBalanceBoardEvent(e); 
  }

  bool GameStateListener::OnTextEvent(const TextEvent& e) 
  { 
    return m_gs->OnTextEvent(e); 
  }

  bool GameStateListener::OnQuitEvent() 
  { 
    return m_gs->OnQuitEvent(); 
  }

  bool GameStateListener::OnResizeEvent(const ResizeEvent& e) 
  { 
    return m_gs->OnResizeEvent(e); 
  }
}

