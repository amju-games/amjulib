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
  m_particles.push_back(new Particle);
  return m_particles.size() - 1;
}

int SpringSystem::CreateSpring(int particleId1, int particleId2)
{
  m_springs.push_back(new Spring(GetParticle(particleId1), GetParticle(particleId2)));
  return m_springs.size() - 1;
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