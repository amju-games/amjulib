#ifndef PARTICLE_EFFECT_2D_H
#define PARTICLE_EFFECT_2D_H

#include "SceneNode.h"
#include "AmjuGL.h"
#include "Texture.h"

namespace Amju
{
struct Particle2d
{
  Vec3f m_pos;
  Vec3f m_vel;
  Vec3f m_acc;
  float m_time;
  bool m_isDead; // if true, this particle is dead, no longer drawn

  Particle2d() : m_time(0), m_isDead(false) {}
};

bool operator<(const Particle2d& p1, const Particle2d& p2);

class ParticleEffect2d : public SceneNode
{
public:
  ParticleEffect2d();
  virtual bool Load(File*);
  virtual void Draw();
  virtual void Update();

  // Call to start effect
  void Start(); 

  // Set up functions, called for each particle on Reset
  virtual Vec3f NewPos() const;
  virtual Vec3f NewVel() const;
  virtual Vec3f NewAcc() const;
  virtual float NewTime() const;
  // Called when a particle time expires
  virtual void HandleDeadParticle(Particle2d*);

protected:
  // Call from HandleDeadParticle to recycle 
  void Recycle(Particle2d*);

private:
  float m_size; // side length of square billboards
  PTexture m_texture;
  typedef std::vector<Particle2d> Particles;
  Particles m_particles;
  AmjuGL::Tris m_tris;
  int m_numParticles;
  // Max time for all particles - after this, HandleDeadParticle is called
  float m_maxTime;
  float m_isDead; // if true, all particles are dead
};
}

#endif

