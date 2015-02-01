#ifndef GS_INITIALISE_H_INCLUDED
#define GS_INITIALISE_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSInitialise : public GSBase
{
  GSInitialise();
  friend class Singleton<GSInitialise>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  
  virtual void CreateTweakMenu() override {}
};
typedef Singleton<GSInitialise> TheGSInitialise;
} // namespace
#endif
