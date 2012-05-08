#ifndef JOYSTICK_TO_CURSOR_H_INCLUDED
#define JOYSTICK_TO_CURSOR_H_INCLUDED

#include <EventListener.h>
#include <Vec2.h>

namespace Amju
{
class JoystickToCursor : public EventListener
{
public:
  virtual bool OnJoyAxisEvent(const JoyAxisEvent&);
  virtual bool OnButtonEvent(const ButtonEvent&);

private:
  Vec2f m_coord;  
};
}

#endif

