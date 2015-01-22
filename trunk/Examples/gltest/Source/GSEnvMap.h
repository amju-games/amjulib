#ifndef GS_ENVMAP_H_INCLUDED
#define GS_ENVMAP_H_INCLUDED

#include <Singleton.h>
#include <EventListener.h>
#include "GSBase.h"

namespace Amju 
{
class GSEnvMap : public GSBase
{
  GSEnvMap();
  friend class Singleton<GSEnvMap>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSEnvMap> TheGSEnvMap;
} // namespace
#endif
