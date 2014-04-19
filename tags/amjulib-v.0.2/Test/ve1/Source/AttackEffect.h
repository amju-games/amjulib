#ifndef ATTACKEFFECT_H_INCLUDED
#define ATTACKEFFECT_H_INCLUDED

#include <ParticleEffect2d.h>

namespace Amju 
{
class AttackEffect : public ParticleEffect2d
{
public:
  virtual Vec3f NewVel() const;
  virtual float NewTime() const;
  virtual void HandleDeadParticle(Particle2d*);
};

} // namespace
#endif
