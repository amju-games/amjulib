#ifndef GS_WATER2_H_INCLUDED
#define GS_WATER2_H_INCLUDED

#include "GSBase.h"
#include "SceneNodeGrid.h"
#include <Singleton.h>

namespace Amju 
{
class GSWater2 : public GSBase
{
  GSWater2();
  friend class Singleton<GSWater2>;

public:
  virtual void Update() override;
  virtual void DrawScene() override;
  virtual void DrawScene2d() override;
  virtual void CreateTweakMenu() override;
  virtual void OnActive() override;

  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;

protected:
  SceneNodeGrid* m_grid;
};
typedef Singleton<GSWater2> TheGSWater2;
} // namespace
#endif
