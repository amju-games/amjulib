#ifndef AMJU_EVENT_LISTENER_H_INCLUDED
#define AMJU_EVENT_LISTENER_H_INCLUDED

#include <set>

namespace Amju
{

enum KeyType
{
  AMJU_KEY_CHAR,  // printable character
  AMJU_KEY_UP,    // up arrow
  AMJU_KEY_DOWN,
  AMJU_KEY_LEFT,
  AMJU_KEY_RIGHT,

  AMJU_KEY_MAX
};

struct KeyEvent
{
  KeyType keyType; // If AMJU_KEY_CHAR, key is a printable character
  char key; // character if key is printable character
  bool keyDown;
};

struct CursorEvent
{
  int x, y;
  short controller;
};

struct JoyAxisEvent
{
  JoyAxisEvent() : x(0), y(0), controller(0) {}

  float x, y; // -1.0 .. 1.0
  short controller;
};

enum Button
{
  AMJU_BUTTON_A,
  AMJU_BUTTON_B,
  AMJU_BUTTON_C,
  AMJU_BUTTON_D,
  AMJU_BUTTON_LEFT,
  AMJU_BUTTON_RIGHT,
  AMJU_BUTTON_UP,
  AMJU_BUTTON_DOWN,

  AMJU_BUTTON_MAX  
};

struct ButtonEvent
{
  Button button;
  bool isDown;
  short controller;
};

enum MouseButton
{
  AMJU_BUTTON_MOUSE_LEFT,
  AMJU_BUTTON_MOUSE_RIGHT,
  AMJU_BUTTON_MOUSE_MIDDLE
};

struct MouseButtonEvent
{
  MouseButton button;
  bool isDown;
};

enum Axis { AMJU_AXIS_X, AMJU_AXIS_Y, AMJU_AXIS_Z };
struct RotationEvent
{
  int controller;
  Axis axis;
  float degs;
};

// TODO other members - needs calibration, mass ?
struct BalanceBoardEvent
{
  int x, y;
};

class EventListener
{
public:
  virtual ~EventListener();
  
  virtual void OnKeyEvent(const KeyEvent&) {}
  virtual void OnCursorEvent(const CursorEvent&) {}
  virtual void OnRotationEvent(const RotationEvent&) {}
  virtual void OnJoyAxisEvent(const JoyAxisEvent&) {}
  virtual void OnButtonEvent(const ButtonEvent&) {}
  virtual void OnMouseButtonEvent(const MouseButtonEvent&) {}
  virtual void OnBalanceBoardEvent(const BalanceBoardEvent&) {}
  virtual void OnQuitEvent() {}
};

typedef std::set<EventListener*> Listeners;  
}

#endif // AMJU_EVENT_LISTENER_H_INCLUDED

