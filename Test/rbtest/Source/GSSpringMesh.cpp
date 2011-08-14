#include "GSSpringMesh.h"
#include <AmjuGL.h>
#include <Game.h>

namespace Amju
{
void GSSpringMesh::Setup()
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

GSSpringMesh::GSSpringMesh()
{
  //m_nextState=...
}

void GSSpringMesh::Update()
{
//  GSBase::Update();

}

void GSSpringMesh::Draw()
{
  TheGame::Instance()->SetClearColour(Colour(1, 0, 0, 1));
  GSBase::Draw();

  m_ss.Draw();
}

void GSSpringMesh::Draw2d()
{
}

void GSSpringMesh::OnActive()
{
  GSBase::OnActive();
  Setup();
}

bool GSSpringMesh::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSSpringMesh::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
