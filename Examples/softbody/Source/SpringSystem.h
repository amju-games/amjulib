#ifndef SPRING_SYSTEM_H
#define SPRING_SYSTEM_H

#include <RCPtr.h>
#include "Particle.h"
#include "Spring.h"

namespace Amju
{
class SpringSystem : public RefCounted 
{
public:
  virtual ~SpringSystem() {}

  virtual void Update();
  virtual void Draw(); // ?

  // Creates a new particle - returns its ID
  int CreateParticle();

  // Creates a new spring connecting the two particles with the given IDs.
  // Returns ID of spring
  int CreateSpring(int particleId1, int particleId2);

  Particle* GetParticle(int id);

  Spring* GetSpring(int id);

  // Spring already exists between two particles? Returns Spring or nullptr
  Spring* GetSpring(int particleId1, int particleId2);

  int GetNumParticles() const;
  int GetNumSprings() const;

protected:
  Particles m_particles;
  Springs m_springs;

};
}

#endif