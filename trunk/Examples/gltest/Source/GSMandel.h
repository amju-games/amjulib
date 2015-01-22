#pragma once

#include <Singleton.h>
#include <TriList.h>
#include "GSBase.h"

namespace Amju 
{
class GSMandel : public GSBase
{
  GSMandel();
  friend class Singleton<GSMandel>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

private:
  RCPtr<TriListStatic> m_triList;
};
typedef Singleton<GSMandel> TheGSMandel;
} // namespace

