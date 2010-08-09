#ifndef AMJU_EVENT_TYPES_H
#define AMJU_EVENT_TYPES_H

/*
  Coords for cursor events etc use this coord system:

    (-1, 1) +--------------+ (1, 1)
            |              |
            |              |
            |              |
   (-1, -1) +--------------+ (1, -1)
*/

namespace Amju
{
class EventListener;

struct Event
{
  virtual ~Event() {}
  virtual void UpdateListener(EventListener*) = 0;
};

enum KeyType
{
  AMJU_KEY_CHAR,  // printable character
  AMJU_KEY_UP,    // up arrow
  AMJU_KEY_DOWN,
  AMJU_KEY_LEFT,
  AMJU_KEY_RIGHT,
  AMJU_KEY_ENTER,
  AMJU_KEY_SPACE,
  AMJU_KEY_ESC,

  AMJU_KEY_MAX
};

struct KeyEvent : public Event
{
  KeyType keyType; // If AMJU_KEY_CHAR, key is a printable character
  char key; // character if key is printable character
  bool keyDown;

  virtual void UpdateListener(EventListener*);
};

struct CursorEvent : public Event
{
  // Cursor coords are -1..1 in x and y
  float x, y;
  short controller;

  virtual void UpdateListener(EventListener*);
};

struct JoyAxisEvent : public Event
{
  JoyAxisEvent() : x(0), y(0), controller(0) {}

  float x, y; // -1.0 .. 1.0
  short controller;

  virtual void UpdateListener(EventListener*);
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

struct ButtonEvent : public Event
{
  Button button;
  bool isDown;
  short controller;

  virtual void UpdateListener(EventListener*);
};

enum MouseButton
{
  AMJU_BUTTON_MOUSE_LEFT,
  AMJU_BUTTON_MOUSE_RIGHT,
  AMJU_BUTTON_MOUSE_MIDDLE
};

struct MouseButtonEvent : public Event
{
  MouseButton button;
  bool isDown;

  virtual void UpdateListener(EventListener*);
};

enum Axis { AMJU_AXIS_X, AMJU_AXIS_Y, AMJU_AXIS_Z };
struct RotationEvent : public Event
{
  int controller;
  Axis axis;
  float degs;

  virtual void UpdateListener(EventListener*);
};

// TODO other members - needs calibration, mass ?
struct BalanceBoardEvent : public Event
{
  // Coords are -1..1 in x and y axes
  float x, y;

  virtual void UpdateListener(EventListener*);
};
}

#endif