#ifndef GS_TITLE_H_INCLUDED
#define GS_TITLE_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSTitle : public GSGui
{
  GSTitle();
  friend class Singleton<GSTitle>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSTitle> TheGSTitle;
} // namespace
#endif
