#include "GSShadow.h"
#include <AmjuGL.h>
#include <SceneGraph.h>
#include <SceneNode.h>
#include <DrawOBB3.h>
#include <Teapot.h>
#include <ShadowMapOpenGL1.h>
#include <ShadowMapOpenGL2.h>

namespace Amju
{
class ShadowSG : public SceneGraph
{
  ShadowMap* m_shadowMap; // TODO RCPtr ?

public:
  ShadowSG(ShadowMap* sm) : m_shadowMap(sm) {}

  void DrawShadows(const Vec3f& lp) // lightpos
  {
    Assert(m_shadowMap);

    m_shadowMap->SetLightPos(AmjuGL::Vec3(lp.x, lp.y, lp.z));

    m_shadowMap->Draw();
  }
};

void ShadowMapDrawFunc();

ShadowSG* GetSG()
{
  static ShadowSG* sg = 0;
  if (!sg)
  {
    ShadowMap* sm = new ShadowMapOpenGL2; 
    sm->SetDrawFunc(ShadowMapDrawFunc);
    sm->Init();
    sg = new ShadowSG(sm);
  }

  return sg;
}

void ShadowMapDrawFunc()
{
  GetSG()->Draw(); // i.e. draw this scene graph
}


class MyNode : public SceneNode
{
public:
  virtual void Draw()
  {
    static OBB3 obb;
    obb.SetExtents(Vec3f(10, 1, 10));
    obb.SetCentre(Vec3f(0, -2.0f, 0));

    AmjuGL::SetColour(Colour(1, 0, 0, 1));
    DrawSolidOBB3(obb);

    static Teapot tp;
    AmjuGL::SetColour(Colour(1, 1, 0, 1));
    tp.Draw();
  }
};

GSShadow::GSShadow()
{
  //m_nextState=...
}

void GSShadow::Update()
{
  //GSBase::Update();

}

void GSShadow::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 1, 1));

  GSBase::Draw();

  Vec3f lightpos(5, 5, 5);
  GetSG()->DrawShadows(lightpos);
}

void GSShadow::Draw2d()
{
}

void GSShadow::OnActive()
{
  GetSG()->Clear();
  GetSG()->SetRootNode(SceneGraph::AMJU_OPAQUE, new MyNode);
  
}

bool GSShadow::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSShadow::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
