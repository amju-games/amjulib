#include "SpringSystem.h"

namespace Amju
{
void SpringSystem::Update()
{
  for (unsigned int i = 0; i < m_springs.size(); i++)
  {
    m_springs[i]->Update();
  }

  for (unsigned int i = 0; i < m_particles.size(); i++)
  {
    m_particles[i]->Update();
  }
}

void SpringSystem::Draw()
{
  for (unsigned int i = 0; i < m_particles.size(); i++)
  {
    m_particles[i]->Draw();
  }

  for (unsigned int i = 0; i < m_springs.size(); i++)
  {
    m_springs[i]->Draw();
  }
}

int SpringSystem::CreateParticle()
{
  int n = m_particles.size();
  m_particles.push_back(new Particle(n));
  return n;
}

int SpringSystem::CreateSpring(int particleId1, int particleId2)
{
  Spring* spr = GetSpring(particleId1, particleId2);
  if (spr)
  {
    return spr->GetId();
  }

  int n = m_springs.size();
  m_springs.push_back(new Spring(n, GetParticle(particleId1), GetParticle(particleId2)));
  return n;
}

Spring* SpringSystem::GetSpring(int particleId1, int particleId2)
{
  for (auto it = m_springs.begin(); it != m_springs.end(); ++it)
  {
    Spring* spr = *it;
    Assert(spr);
    Particle* p1 = spr->GetParticle(0);
    Particle* p2 = spr->GetParticle(1);
    Assert(p1);
    Assert(p2);
    int id1 = p1->GetId();
    int id2 = p2->GetId();
    if ((id1 == particleId1 && id2 == particleId2) || (id2 == particleId1 && id1 == particleId2))
    {
      return spr;
    }
  }

  return nullptr;
}

int SpringSystem::GetNumParticles() const
{
  return m_particles.size();
}

int SpringSystem::GetNumSprings() const
{
   return m_springs.size();
}


Particle* SpringSystem::GetParticle(int id)
{
  Assert((unsigned int)id < m_particles.size());
  return m_particles[id];
}

Spring* SpringSystem::GetSpring(int id)
{
  Assert((unsigned int)id < m_springs.size());
  return m_springs[id];
}
}