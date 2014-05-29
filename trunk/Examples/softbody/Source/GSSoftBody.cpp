#include <ResourceManager.h>
#include <Unproject.h>
#include "GSSoftbody.h"
#include "Squishy.h"

namespace Amju
{
static RCPtr<Squishy> sq = 0; // TODO quick hack so we can easily reset
static float yrot = 0;
static float xrot = 0;
static bool dragleft = false;
static bool dragright = false;
static Vec2f mousePos;

enum CutMode { CUT_NONE, CUT_START, CUT_CONTINUE, CUT_END };
static CutMode cutmode = CUT_NONE;

LineSeg MouseToLineSeg(const Vec2f& mouseScreen)
{
  Vec3f mouseWorldNear;
  Vec3f mouseWorldFar;

  Unproject(mouseScreen, 0, &mouseWorldNear);
  Unproject(mouseScreen, 1, &mouseWorldFar);

  return LineSeg(mouseWorldNear, mouseWorldFar);
}

GSSoftBody::GSSoftBody()
{
}

void GSSoftBody::Update()
{
  sq->Update();

}

void GSSoftBody::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 1, 1));
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetPerspectiveProjection(45.0f, 1.3f, 1.0f, 1000.0f); //FOVY, ASPECT, NEAR, FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::LookAt(0, 0, 50,  0, 0, 0,   0, 1, 0);

  Vec3f pos(1, 1, 1);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(pos.x, pos.y, pos.z)); // Light direction

  AmjuGL::Translate(0, -10, 0);

  AmjuGL::RotateY(yrot);
  AmjuGL::RotateX(xrot);

  sq->Draw();

  LineSeg seg = MouseToLineSeg(mousePos);
//    std::cout << "Mouse pos: " << mousePos.x << " " << mousePos.y << "\n";
  switch (cutmode)
  {
  case CUT_START:
    sq->StartCut(seg, 0);
    cutmode = CUT_CONTINUE;
    break;
    
  case CUT_CONTINUE:
    sq->ContinueCut(seg, 0);
    break;
    
  case CUT_END:
    sq->EndCut(seg, 0);
    cutmode = CUT_NONE;
    break;

  default:
    break;
  }
}

void GSSoftBody::Draw2d()
{
}

void GSSoftBody::OnActive()
{
  cutmode = CUT_NONE;

  // K value is how soft/tough
  const float K = 1.0f; // TODO
  const std::string filename = "bean.obj"; // crate2.obj

  sq = new Squishy;
  if (!sq->Init(filename, K))
  {
    std::cout << "Failed to initialise squishy from obj mesh!\n";
    Assert(0);
  }
}

bool GSSoftBody::OnCursorEvent(const CursorEvent& ce)
{
  mousePos.x = ce.x;
  mousePos.y = ce.y;

  if (dragright)
  {
    xrot += ce.dy * 100.0f;
    yrot += ce.dx * 100.0f;
  }

  return false;
}

bool GSSoftBody::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    dragleft = mbe.isDown;  
    if (dragleft)
    {
      cutmode = CUT_START;
    }
    else
    {
      cutmode = CUT_END;
    }
  }
  if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT)
  {
    dragright = mbe.isDown;  
  }
  return false;
}

bool GSSoftBody::OnKeyEvent(const KeyEvent& ke)
{
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'r')
  {
    // Reset sim
    OnActive();
  }
 
  // TODO Modes: cut, select, clear, rotate camera, etc. 
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'p')
  {
    // TODO TEMP TEST
    int r = rand() % sq->GetNumParticles();
    sq->GetParticle(r)->AddForce(Vec3f(100, 0, 0));
  }

/*
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == '2')
  {
    MakeBox2(Vec2f(3.0f, 20.0), 0.3f);
  }
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == '3')
  {
std::cout << "Adding a box!\n";

    MakeBox3(Vec3f(3.0f, 20.0, 0));
  }
  */

  return true; // handled
}
}
