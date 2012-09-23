#ifndef AMJU_EVENT_TYPES_H
#define AMJU_EVENT_TYPES_H

#include <string>

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
  // Return true if listener eats event
  virtual bool UpdateListener(EventListener*) = 0;
};

struct QuitEvent : public Event
{
  virtual bool UpdateListener(EventListener*);
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
  AMJU_KEY_BACKSPACE,
  AMJU_KEY_DELETE,
  AMJU_KEY_PAGE_UP,
  AMJU_KEY_PAGE_DOWN,
  AMJU_KEY_HOME,
  AMJU_KEY_END,
  AMJU_KEY_ALT,

  AMJU_KEY_MAX
};

enum KeyModifier
{  
  AMJU_KEY_MOD_NONE = 0,
  AMJU_KEY_MOD_SHIFT = 1,
  AMJU_KEY_MOD_CTRL = 2,
  AMJU_KEY_MOD_ALT = 4
};

struct KeyEvent : public Event
{
  KeyEvent() : keyDown(false), modifier(0) {}

  KeyType keyType; // If AMJU_KEY_CHAR, key is a printable character
  char key; // character if key is printable character, TODO UNICODE
  bool keyDown;
  unsigned int  modifier; // bitwise combo of KeyModifiers

  virtual bool UpdateListener(EventListener*);
};

struct CursorEvent : public Event
{
  // Cursor coords are -1..1 in x and y
  float x, y;
  short controller;

  virtual bool UpdateListener(EventListener*);
};

struct JoyAxisEvent : public Event
{
  JoyAxisEvent() : x(0), y(0), controller(0) {}

  float x, y; // -1.0 .. 1.0
  short controller;

  virtual bool UpdateListener(EventListener*);
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

  virtual bool UpdateListener(EventListener*);
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
  // Useful to give coords. Cursor coords are -1..1 in x and y
  float x, y;

  virtual bool UpdateListener(EventListener*);
};

struct DoubleClickEvent : public Event
{
  MouseButton button;
  // Useful to give coords. Cursor coords are -1..1 in x and y
  float x, y;

  virtual bool UpdateListener(EventListener*);
};

enum Axis { AMJU_AXIS_X, AMJU_AXIS_Y, AMJU_AXIS_Z };
struct RotationEvent : public Event
{
  int controller;
  Axis axis;
  float degs;

  virtual bool UpdateListener(EventListener*);
};

// TODO other members - needs calibration, mass ?
struct BalanceBoardEvent : public Event
{
  // Coords are -1..1 in x and y axes
  float x, y;

  virtual bool UpdateListener(EventListener*);
};

enum TextEventType { AMJU_OOPS, AMJU_CUT, AMJU_COPY, AMJU_PASTE, AMJU_UNDO, AMJU_REDO };
struct TextEvent : public Event
{
  TextEvent() : type(AMJU_OOPS), clipboard(0) {}
  TextEventType type;
  std::string* clipboard;

  virtual bool UpdateListener(EventListener*);
};

enum ResizeType { AMJU_MAXIMISE, AMJU_MINIMISE, AMJU_RESIZE, AMJU_FULLSCREEN_ON, AMJU_FULLSCREEN_OFF };
struct ResizeEvent : public Event
{
  int x, y;
  ResizeType type; 
  
  virtual bool UpdateListener(EventListener*);
};

}

#endif
