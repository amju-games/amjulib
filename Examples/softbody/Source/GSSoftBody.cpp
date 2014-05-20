#include <ResourceManager.h>
#include "GSSoftbody.h"
#include "Squishy.h"

namespace Amju
{
static RCPtr<Squishy> sq = 0; // TODO quick hack so we can easily reset
static float yrot = 0;
static float xrot = 0;
static bool drag = false;

GSSoftBody::GSSoftBody()
{
}

void GSSoftBody::Update()
{
  sq->Update();

  // TODO TEMP TEST
//  sq->GetParticle(0)->AddForce(Vec3f(1, 0, 0));
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
}

void GSSoftBody::Draw2d()
{
}

void GSSoftBody::OnActive()
{
  // K value is how soft/tough
  const float K = 1.0f; // TODO

  sq = new Squishy;
  if (!sq->Init("crate2.obj", K))
  {
    std::cout << "Failed to initialise squishy from obj mesh!\n";
    Assert(0);
  }
}

bool GSSoftBody::OnCursorEvent(const CursorEvent& ce)
{
  if (drag)
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
    drag = mbe.isDown;  
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
  /*
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'p')
  {
    m_paused = !m_paused;
  }
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
