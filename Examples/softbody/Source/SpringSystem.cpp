#include <iostream>
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

Particle* SpringSystem::CreateParticle()
{
  int n = m_particles.size();
  Particle* p = new Particle(n);
  m_particles.push_back(p);
  return p;
}

void SpringSystem::GetSprings(int particleId, SpringSet* springs)
{
  *springs = m_springMap[particleId];
}

int SpringSystem::CreateSpring(int particleId1, int particleId2)
{
  Spring* spr = GetSpring(particleId1, particleId2);
  if (spr)
  {
    std::cout << "CreateSpring: found duplicate spring.\n";
    return spr->GetId();
  }

  int n = m_springs.size();
  spr = new Spring(n, GetParticle(particleId1), GetParticle(particleId2));
  m_springs.push_back(spr);

  // Add this spring to the map of springs connected to each particle
  m_springMap[particleId1].insert(spr);
  m_springMap[particleId2].insert(spr);

  // We have just set the natural length of the spring.
  // Set the min and max lengths based on this.
  // TODO This is a property of the squishy thing
  float natLen = spr->GetNaturalLength();
  // TODO TEMP TEST
  spr->SetMaxLength(natLen * 1.5f);
  spr->SetMinLength(natLen * 0.7f);
  return n;
}

bool SpringSystem::EraseSpring(int particleId1, int particleId2)
{
  Spring* spr = GetSpring(particleId1, particleId2);
  if (spr)
  {
    m_springs.erase(std::remove(m_springs.begin(), m_springs.end(), spr), m_springs.end());

    m_springMap[particleId1].erase(spr);
    m_springMap[particleId2].erase(spr);
 
    return true;
  }
  return false;
}

void SpringSystem::EraseParticle(int particleId)
{
  // TODO
  Assert(0);
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
