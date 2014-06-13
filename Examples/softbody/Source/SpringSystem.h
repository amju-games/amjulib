#ifndef SPRING_SYSTEM_H
#define SPRING_SYSTEM_H

#include <map>
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

  // Creates a new particle - returns ptr to it (Particle has GetId())
  Particle* CreateParticle();

  // Creates a new spring connecting the two particles with the given IDs.
  // Returns ID of spring
  int CreateSpring(int particleId1, int particleId2);

  Particle* GetParticle(int id);

  Spring* GetSpring(int id);

  // Spring already exists between two particles? Returns Spring or nullptr
  Spring* GetSpring(int particleId1, int particleId2);

  // Populate springs vector with all the springs connected to the given
  //  particle
  void GetSprings(int particleId, SpringSet* springs);

  // Just erases the spring - returns true if spring existed and was deleted,
  //  false if no such spring.
  bool EraseSpring(int particleId1, int particleId2);

  // Erase particle and all springs which connect to it
  void EraseParticle(int particleId);

  int GetNumParticles() const;
  int GetNumSprings() const;

private:
  Particles m_particles;
  Springs m_springs;

  // Map of particle ID to springs connected to it, in case we need to know 
  //  which springs are connected to a particle
  typedef std::map<int, SpringSet> SpringMap;
  SpringMap m_springMap;
};
}

#endif
