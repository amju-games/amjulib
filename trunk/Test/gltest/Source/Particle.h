#ifndef PARTICLE_H
#define PARTICLE_H

#include <Vec3.h>
#include <vector>
#include <RCPtr.h>

namespace Amju
{
class Particle : public RefCounted
{
public:
  Particle();

  void Draw();
  void Update();

  void AddForce(const Vec3f& force);
  void ResetForces(); // Called at end of Update anyway

  void SetPos(const Vec3f& pos);
  const Vec3f& GetPos() const;
  // Move this much if not immovable
  void Particle::Move(const Vec3f& v);

  void SetInvMass(float invMass);

private:
  Vec3f m_pos;
  Vec3f m_vel;
  Vec3f m_acc;
  float m_invMass;
  Vec3f m_forces; // accumulated forces added together (D'Alembert)
};

typedef RCPtr<Particle> PParticle;
typedef std::vector<PParticle> Particles;
}

#endif
