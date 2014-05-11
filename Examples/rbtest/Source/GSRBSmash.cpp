#include "GSRBSmash.h"
#include <iostream>
#include <Game.h>
#include <AmjuGL.h>
#include <Font.h>
#include <StringUtils.h>
#include <DegRad.h>
#include <AmjuGL.h>
#include "RBBox3.h"
#include "RBBox2.h"
#include "RBSphere3.h"
#include "RBSphere2.h"
#include "RBManager.h"
#include "RBSceneNode.h"
#include <SceneGraph.h>
#include <StringUtils.h>
#include <Timer.h>
#include <ShadowMap.h>

#define USE_SHADOWMAP

namespace Amju
{
static SceneGraph* GetRBSceneGraph()
{
  static SceneGraph* sg = 0;
  if (!sg)
  {
    sg = new SceneGraph;
  }
  return sg;
}

static void ShadowDraw()
{
  GetRBSceneGraph()->Draw();
}

static void AddToSceneGraph(RB* rb)
{
  SceneNode* root = GetRBSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  root->AddChild(new RBSceneNode(rb));
}

GSRBSmash::GSRBSmash()
{
}

void GSRBSmash::Update()
{
  TheRBManager::Instance()->Update();
}

void GSRBSmash::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_BLEND);

  GSBase::Draw();

#ifdef USE_SHADOWMAP

  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

  static PShadowMap sm = 0;
  if (!sm)
  {
    sm = (ShadowMap*)AmjuGL::Create(ShadowMap::DRAWABLE_TYPE_ID);
    sm->SetLightPos(AmjuGL::Vec3(20, 20, 20));
    sm->Init();
    sm->SetDrawFunc(ShadowDraw);
  }
  sm->Draw();

#else

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);

  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(1, 1, 1)); // Light direction

  GetRBSceneGraph()->Draw();

#endif
}

void GSRBSmash::Draw2d()
{
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  AmjuGL::SetColour(Colour(1, 1, 1, 1));
}


static RBBox3* MakeBox3(const Vec3f& pos)
{
  RBBox3* rb = new RBBox3;
  rb->SetPos(pos);
  //rb->SetSize(Vec3f(2, 2, 2));

  rb->SetSize(Vec3f(1, 1, 1));
  // Work out mass from volume ?

  //Quaternion q;
  //float angle = (float)(rand() % 360);
  //std::cout << "Angle :" << angle << " degs\n";
  //q.SetAxisAngle(DegToRad(angle), Vec3f(0, 0, 1.0f));
  //rb->SetRot(q);

  TheRBManager::Instance()->AddRB(rb);

  AddToSceneGraph(rb);

  return rb;
}


void GSRBSmash::OnActive()
{
  Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");
  Assert(font);
//  TheGame::Instance()->SetFrameTimeFont(font);

  TheRBManager::Instance()->Clear();

  GSBase::OnActive();
   
  GetRBSceneGraph()->Clear(); 
  GetRBSceneGraph()->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);

  MakeBox3(Vec3f(0, 1.0f, 0));
  MakeBox3(Vec3f(3, 1.0f, 0));
  MakeBox3(Vec3f(3, 1.0f, 3));
  MakeBox3(Vec3f(0, 1.0f, 3));

  MakeBox3(Vec3f(1.5f, 4.0f, 1.5f));

  MakeBox3(Vec3f(0, 7.0f, 0));
  MakeBox3(Vec3f(3, 7.0f, 0));
  MakeBox3(Vec3f(3, 7.0f, 3));
  MakeBox3(Vec3f(0, 7.0f, 3));

  MakeBox3(Vec3f(1.5f, 10.0f, 1.5f));

  // Big base
  const float S = 20.0f;
  RBBox3* rb = new RBBox3;
  rb->SetPos(Vec3f(0, -S, 0));
  rb->SetSize(Vec3f(S, S, S));
  rb->SetInvMass(0); // immovable
  TheRBManager::Instance()->AddRB(rb);
  AddToSceneGraph(rb);


}

bool GSRBSmash::OnKeyEvent(const KeyEvent& ke)
{
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'r')
  {
    // Reset sim
    OnActive();
  }
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == '3')
  {
std::cout << "Adding a box!\n";

    MakeBox3(Vec3f(3.0f, 20.0, 0));
  }

  return true; // handled
}

}
