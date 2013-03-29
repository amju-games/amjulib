#include <AmjuFirst.h>
#include "TextDraw.h"
#include <SceneMesh.h>
#include "Ve1SceneGraph.h"
#include <CollisionMesh.h>
#include <AABB.h>
#include "MyTextMaker.h"
#include <DegRad.h>
#include <Timer.h>
#include <Localise.h>
#include <Screen.h>
#include <Game.h>
#include <AmjuFinal.h>

namespace Amju
{
void TextDraw::Update()
{
  GetGuiSceneGraph()->Update();
}

void TextDraw::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR = 1.0f; 
  const float FAR = 3000.0f;
  static const float aspect = (float)Screen::X() / (float)Screen::Y();
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR, FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::PushMatrix();
  static float a = 0; 
  // TODO CONFIG!!
  a += TheTimer::Instance()->GetDt(); 
  // TODO z distance
  AmjuGL::LookAt(cos(a), 2.0f + sin(a), 8.0f,   0, 0, 0.0f,  0, 1.0f, 0);

  // TODO Lighting node
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(1, 1, 1)); // Light direction

  GetGuiSceneGraph()->Draw();
  AmjuGL::PopMatrix();
}

class Reflect : public SceneNode
{
public:
  virtual void BeforeDraw()
  {
    PushColour();
    static const float s = 0.5f;
    MultColour(Colour(s, s, s, 1));
    AmjuGL::PushMatrix();
    // TODO Doesn't look right
    AmjuGL::Scale(1, -1, 1);
    AmjuGL::Translate(0, -2.0f, 0);
  }

  virtual void AfterDraw()
  {
    AmjuGL::PopMatrix();
    PopColour();
  }
};

void TextDraw::CreateText(const std::string& text)
{
  MyTextMaker tm;
  PSceneNode node = tm.MakeText(Lookup(text));

  // Rotate
  Matrix m;
  m.RotateX(DegToRad(60.0f));
  node->MultLocalTransform(m);
  // TODO Should combine ?
  node->CombineTransform();
  node->RecursivelyTransformAABB(m);

  SceneNode* parent = new SceneNode;
  parent->AddChild(node);
  parent->UpdateBoundingVol();

  /*
  // Reflection - TODO
  Reflect* reflect = new Reflect;
  parent->AddChild(reflect);
  reflect->AddChild(node);
  */

  GetGuiSceneGraph()->SetRootNode(SceneGraph::AMJU_OPAQUE, parent);
}
}
