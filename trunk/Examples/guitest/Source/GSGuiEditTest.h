#ifndef GS_GUIEDITTEST_H_INCLUDED
#define GS_GUIEDITTEST_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
// Test text edit GUI elements
class GSGuiEditTest : public GSGui
{
  GSGuiEditTest();
  friend class Singleton<GSGuiEditTest>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSGuiEditTest> TheGSGuiEditTest;
} // namespace
#endif
