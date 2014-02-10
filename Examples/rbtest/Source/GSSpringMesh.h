#ifndef GS_SPRINGMESH_H_INCLUDED
#define GS_SPRINGMESH_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"
#include "SpringSystem.h"

namespace Amju 
{
class GSSpringMesh : public GSBase
{
  GSSpringMesh();
  friend class Singleton<GSSpringMesh>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

private:
  void Setup();

  SpringSystem m_ss;
};
typedef Singleton<GSSpringMesh> TheGSSpringMesh;
} // namespace
#endif
