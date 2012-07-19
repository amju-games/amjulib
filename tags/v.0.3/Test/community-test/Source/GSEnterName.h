#ifndef GS_ENTER_NAME_H
#define GS_ENTER_NAME_H

#include <GameState.h>
#include <GuiElement.h>
#include <Singleton.h>

namespace Amju
{
class GSEnterName : public GameState
{
  GSEnterName() {}
  friend class Singleton<GSEnterName>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();
  
protected:
  PGuiElement m_gui;  
};

typedef Singleton<GSEnterName> TheGSEnterName;
}

#endif

