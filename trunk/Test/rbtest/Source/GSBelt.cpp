#include "GSBelt.h"
#include <Game.h>
#include <AmjuGL.h>
#include <Font.h>
#include <StringUtils.h>

#ifdef WIN32
#define _USE_MATH_DEFINES // ffs
#endif // WIN32

#include <math.h>

namespace Amju
{
GSBelt::GSBelt()
{
  m_isMouseDown = false;
}

bool GSBelt::OnCursorEvent(const CursorEvent& ce)
{
  static float oldx = ce.x;
  static float oldy = ce.y;
  if (m_isMouseDown)
  {
    float xdiff = ce.x - oldx;
    float ydiff = ce.y - oldy;

    Vec3f p = m_ss.GetParticle(0)->GetPos();
    p.x += xdiff * 10.0f;
    p.z += ydiff * 10.0f;
    m_ss.GetParticle(0)->SetPos(p);
  }
  oldx = ce.x;
  oldy = ce.y;

  return false;
}

bool GSBelt::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    m_isMouseDown = mbe.isDown;
  }

  return false;
}

void GSBelt::Update()
{
  m_ss.Update();
}

void GSBelt::Draw()
{
  TheGame::Instance()->SetClearColour(Colour(0, 0, 0, 1));

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR = 1.0f;
  const float FAR = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR, FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  AmjuGL::LookAt(0, 10, 10,  0, 0, 0,  0, 1, 0);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);


  m_ss.Draw();
}

void GSBelt::Draw2d()
{
/*
  static Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");
  Assert(font);

  Vec3f p = m_ss.GetParticle(0)->GetPos();
  std::string s = "X: " + ToString(p.x, 2) + " Y: " + ToString(p.y, 2) + " Z: " + ToString(p.z, 2);

  font->Print(-1, -0.8f, s.c_str());
*/
}

void GSBelt::Set2DSystem2()
{
  float K = 500.0f;
  int NUM_PARTICLES = 10; // in x and z
  float space = 20.0f / (float)NUM_PARTICLES;

  for (int i = 0; i < NUM_PARTICLES; i++)
  {
    for (int j = 0; j < NUM_PARTICLES; j++)
    {
      int particleId = m_ss.CreateParticle();
      Particle* p = m_ss.GetParticle(particleId);
      p->SetPos(Vec3f(
        (float) (i - NUM_PARTICLES / 2) * space,          
        -(float) (j - NUM_PARTICLES / 2) * space, 0));

      if (i == 0 && j == 0 || i == (NUM_PARTICLES - 1) && j == 0)
      {
        p->SetInvMass(0);
      }

      if (i > 0)
      {
        Spring* s = m_ss.GetSpring(
          m_ss.CreateSpring(particleId - NUM_PARTICLES, particleId));
        s->SetK(K);
        s->SetMaxLength(space + 0.01f);
        s->SetMinLength(space);
      }
      
      if (j > 0)
      {
        Spring* s = m_ss.GetSpring(
          m_ss.CreateSpring(particleId - 1, particleId));
        s->SetK(K);
        s->SetMaxLength(space + 0.01f);
        s->SetMinLength(space);
      }
      // Also a diagonal ?

    }
  }
}
 
void GSBelt::Set2DSystem()
{
  int NUM_PARTICLES = 20; // in x and z
  float space = 20.0f / (float)NUM_PARTICLES;

  for (int i = 0; i < NUM_PARTICLES; i++)
  {
    for (int j = 0; j < NUM_PARTICLES; j++)
    {
      int particleId = m_ss.CreateParticle();
      Particle* p = m_ss.GetParticle(particleId);
      p->SetPos(Vec3f(
        (float) (i - NUM_PARTICLES / 2) * space, 
        0, 
        (float) (j - NUM_PARTICLES / 2) * space));

      if (i == 0 && j == 0)
      {
        //p->SetInvMass(0);
      }

      if (i > 0)
      {
        Spring* s = m_ss.GetSpring(
          m_ss.CreateSpring(particleId - NUM_PARTICLES, particleId));
        s->SetK(1.0f);
        s->SetMaxLength(1.01f);
        s->SetMinLength(1.0f);
      }
      
      if (j > 0)
      {
        Spring* s = m_ss.GetSpring(
          m_ss.CreateSpring(particleId - 1, particleId));
        s->SetK(1.0f);
        s->SetMaxLength(1.01f);
        s->SetMinLength(1.0f);
      }
      // Also a diagonal ?

    }
  }
}

void GSBelt::Set1DSystem()
{
  int NUM_PARTICLES = 20;
  float space = 20.0f / (float)NUM_PARTICLES;

  for (int i = 0; i < NUM_PARTICLES; i++)
  {
    Particle* p = m_ss.GetParticle(m_ss.CreateParticle());
    p->SetPos(Vec3f((float) (i - NUM_PARTICLES / 2) * space, 0, 0));
    if (i == 0 || i == NUM_PARTICLES - 1)
    {
      // End points don't move
      p->SetInvMass(0);
    }

    if (i > 0)
    {
      Spring* s = m_ss.GetSpring(m_ss.CreateSpring(i - 1, i));
      s->SetK(100.0f);
      s->SetMaxLength(0.5f);

      //s->ConnectToParticles(m_particles[i - 1], m_particles[i]);

      //Quaternion q(Vec3f(1, 0, 0), 0.0f * (float)M_PI * (float)(i) / (float)(NUM_PARTICLES));
      //s->SetQuat(q);
    }
  }
}

void GSBelt::SetSimpleSystem()
{
    // weight on spring

    // Top of spring - immovable
    Particle* p = m_ss.GetParticle(m_ss.CreateParticle());
    p->SetInvMass(0); // immoveable
    p->SetPos(Vec3f(0, 2.0f, 0));

    p = m_ss.GetParticle(m_ss.CreateParticle());
    p->SetInvMass(0.2f); 
    p->SetPos(Vec3f(0, -2.0f, 0));

    Spring* s = m_ss.GetSpring(m_ss.CreateSpring(0, 1));
    s->SetK(20.0f);

    // Third particle ?

}

void GSBelt::OnActive()
{
  GameState::OnActive();

  m_isMouseDown = false;

  //SetSimpleSystem();
  Set1DSystem();
  //Set2DSystem();
}
}
