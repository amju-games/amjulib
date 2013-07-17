#include <AmjuFirst.h>
#include <AmjuGL.h>
#include "AttackEffect.h"
#include <AmjuFinal.h>

namespace Amju
{
static float rnd(float f)
{
  return ((float)rand() / (float)RAND_MAX * 2.0f * f - f);
}

static const float PARTICLE_SPEED = 50.0f;

void AttackEffect::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::RotateX(-90.0f);
  ParticleEffect2d::Draw();
  AmjuGL::PopMatrix();
}

Vec3f AttackEffect::NewVel() const
{
  return Vec3f(
    rnd(PARTICLE_SPEED),
    rnd(PARTICLE_SPEED),
    rnd(PARTICLE_SPEED));

  // Particles spin so we want to emit them in all directions
  // z always towards us, or will be hidden by billboard
  //(float)rand() / (float)RAND_MAX * PARTICLE_SPEED);
}

float AttackEffect::NewTime() const
{
  return (float)rand() / (float)RAND_MAX * 1.0f;
}

void AttackEffect::HandleDeadParticle(Particle2d* p)
{
//  Recycle(p);
  ParticleEffect2d::HandleDeadParticle(p);
}


} // namespace
