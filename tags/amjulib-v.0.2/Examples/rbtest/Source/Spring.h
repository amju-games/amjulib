#ifndef SPRING_H
#define SPRING_H

#include <vector>
#include <RCPtr.h>
#include <OBB3.h>
#include <Quaternion.h>

namespace Amju
{
class Particle;

// Spring connects two particles, (one may be fixed)
// A particle can have multiple Springs attached
class Spring : public RefCounted
{
public:
  Spring();
  Spring(Particle* p1, Particle* p2);

  void ConnectToParticles(Particle* p1, Particle* p2);

  void Update();
  void Draw();

  void SetQuat(const Quaternion&);
  const Quaternion& GetQuat() const;

  void SetNaturalLength(float len);
  void SetMaxLength(float maxLen);
  void SetMinLength(float minLen);
  void SetK(float k);

private:
  Particle* m_particles[2];
  // float m_K; // TODO per-spring K

  OBB3 m_box;
  Quaternion m_quat; // so we can easily get it
  Vec3f m_oldVec; // last vec between particles
  float m_oldLen; // length last frame, to get delta 
  float m_naturalLength;

  // Max and min spring lengths - ends are pulled/pushed to maintain these
  float m_maxLength; 
  float m_minLength;

  float m_k; // spring constant for Hooke's Law
};

typedef RCPtr<Spring> PSpring;
typedef std::vector<PSpring> Springs;
}

#endif


