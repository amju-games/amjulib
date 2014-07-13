#include <AmjuGL.h>
#include <SceneGraph.h>
#include <SceneNode.h>
#include <DrawOBB3.h>
#include <DrawAABB.h>
#include <Teapot.h>
#include <ShadowMapOpenGL1.h>
#include <ShadowMapOpenGL2.h>
#include <Timer.h>
#include "GSShadow.h"
#include "GSMandel.h"

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
    ShadowMap* sm = (ShadowMap*)AmjuGL::Create(ShadowMap::DRAWABLE_TYPE_ID);
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
   // SetIsLit(true);

    static OBB3 obb;
    obb.SetExtents(Vec3f(10, 1, 10));
    obb.SetCentre(Vec3f(0, -2.0f, 0));

    AmjuGL::SetColour(Colour(1, 0, 0, 1));
//    DrawSolidOBB3(obb);

    static Teapot tp;
    AmjuGL::SetColour(Colour(1, 1, 0, 1));
    tp.Draw();
  }
};

GSShadow::GSShadow()
{
  m_name = "Shadow map";
  m_nextState = TheGSMandel::Instance();
  m_maxTime = 5.0f; // so awesome
}

void GSShadow::Update()
{
  GSBase::Update();
}


void GSShadow::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 1, 1));

  GSBase::Draw();

  float dt = TheTimer::Instance()->GetDt();

  // MV matrix is set up
  static float r = 0;
//  r += 20.0f * dt;
  AmjuGL::RotateY(r);

  static float s = 0;
  s += dt;

  Vec3f lightpos(20, 20, 20); //5.0f * cos(s), 5.0f, 5.0f * sin(s));
  GetSG()->DrawShadows(lightpos);
}


void GSShadow::OnActive()
{
  GSBase::OnActive();


  GetSG()->Clear();
  GetSG()->SetRootNode(SceneGraph::AMJU_OPAQUE, new MyNode);
  
}

void GSShadow::Draw2d()
{
  GSBase::Draw2d();
}

/*
void GSShadow::Draw()
{
  GSBase::Draw();

  AmjuGL::SetClearColour(Colour(0, 0, 1, 1));

  static float t = 0;
  t += TheTimer::Instance()->GetDt();
  Vec3f pos(cos(t), 1, sin(t));

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(pos.x, pos.y, pos.z)); // Light direction

  static float f = 0;
  f += 0.01f;

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  static AABB aabb(-1, 1, -1, 1, -1, 1);

  AmjuGL::PushMatrix();
  AmjuGL::Translate(-2.0f, 0, 0);
  AmjuGL::RotateY(f);
  DrawSolidAABB(aabb);
  AmjuGL::PopMatrix();

  static Teapot tp;

  AmjuGL::PushMatrix();
  AmjuGL::Translate(2.0f, 0, 0);
  AmjuGL::RotateY(f);
  tp.Draw();
  AmjuGL::PopMatrix();

  // Draw light source
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  static AABB lightAabb(-0.1f, 0.1f, -0.1f, 0.1f, -0.1f, 0.1f);
  AmjuGL::PushMatrix();
  const float dist = 5.0f;
  AmjuGL::Translate(pos.x * dist, 1, pos.z * dist);
  DrawSolidAABB(lightAabb); 
  AmjuGL::PopMatrix();
}
*/

} // namespace
