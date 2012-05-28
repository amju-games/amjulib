#ifndef GS_LETTERCANCELLATION1_H_INCLUDED
#define GS_LETTERCANCELLATION1_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class LCListener : public EventListener
{
public:
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  
};

class GSLetterCancellation1 : public GSGui
{
  GSLetterCancellation1();
  friend class Singleton<GSLetterCancellation1>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

  bool OnCursorEvent(const CursorEvent&);
  bool OnMouseButtonEvent(const MouseButtonEvent&);

protected:
  std::vector<GuiElement*> m_letters;
  RCPtr<EventListener> m_listener;
  unsigned int m_timer;
};
typedef Singleton<GSLetterCancellation1> TheGSLetterCancellation1;
} // namespace
#endif
