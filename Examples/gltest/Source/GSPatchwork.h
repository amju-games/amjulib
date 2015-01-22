#ifndef GS_PATCHWORK_H_INCLUDED
#define GS_PATCHWORK_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSPatchwork : public GSBase
{
  GSPatchwork();
  friend class Singleton<GSPatchwork>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSPatchwork> TheGSPatchwork;
} // namespace
#endif
