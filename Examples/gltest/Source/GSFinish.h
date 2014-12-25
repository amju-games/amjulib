#ifndef GS_FINISH_H
#define GS_FINISH_H

#include <GuiElement.h>
#include "GSBase.h"

namespace Amju
{
class GSFinish : public GSBase
{
public:
  static const char* NAME;

  virtual void Update();
  virtual void DrawScene();
  virtual void Draw2d();
  virtual void OnActive();

protected:
  PGuiElement m_gui;
};

typedef Singleton<GSFinish> TheGSFinish;
}

#endif

