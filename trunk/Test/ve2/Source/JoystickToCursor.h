#ifndef JOYSTICK_TO_CURSOR_H_INCLUDED
#define JOYSTICK_TO_CURSOR_H_INCLUDED

#include <EventListener.h>
#include <Vec2.h>

namespace Amju
{
class JoystickToCursor : public EventListener
{
public:
  JoystickToCursor();

  virtual bool OnJoyAxisEvent(const JoyAxisEvent&);
  virtual bool OnButtonEvent(const ButtonEvent&);

  void Update();

private:
  Vec2f m_joy;
  Vec2f m_coord;  
  bool m_hasEvent; 
};

JoystickToCursor* TheJoystickToCursor();
}

#endif

