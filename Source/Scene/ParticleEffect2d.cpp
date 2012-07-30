#include "ParticleEffect2d.h"
#include "File.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "LoadTextureResource.h"
#include <AmjuGL.h>

namespace Amju
{
bool operator<(const Particle2d& p1, const Particle2d& p2)
{
  // This is only ok because we know the camera is looking parallel to z axis
  return p1.m_pos.z > p2.m_pos.z;
}

ParticleEffect2d::ParticleEffect2d()
{
  m_numParticles = 0;
  m_size = 0;
  m_maxTime = 0;
  m_isDead = true; // ?

  SetBlended(true);
}

bool ParticleEffect2d::Load(File* f)
{
  m_texture = LoadTextureResource(f);

  // TODO Random sizes
  // TODO Also rotate, with random rot vel

  if (!f->GetFloat(&m_size))
  {
    f->ReportError("Expected particle size");
    return false;
  }
  if (!f->GetInteger(&m_numParticles))
  {
    f->ReportError("Expected num particles");
    return false;
  }
  if (!f->GetFloat(&m_maxTime))
  {
    f->ReportError("Expected particle max time");
    return false;
  }

  m_tris.resize(m_numParticles * 2);
  m_particles.resize(m_numParticles);

  return true;
}

void ParticleEffect2d::Draw()
{
  if (m_isDead)
  {
    return;
  }

  if (m_tris.empty())
  {
    return;
  }

  // TODO Also push lighting and blend state ?
  AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_combined); // NB combined
  m_texture->UseThisTexture();
  AmjuGL::DrawTriList(m_tris);
  AmjuGL::PopMatrix();
  AmjuGL::PopAttrib();
}

Vec3f ParticleEffect2d::NewPos() const
{
  return Vec3f();
}

Vec3f ParticleEffect2d::NewVel() const
{
  return Vec3f();
}

Vec3f ParticleEffect2d::NewAcc() const
{
  return Vec3f();
}

float ParticleEffect2d::NewTime() const
{
  return 0;
}

void ParticleEffect2d::Recycle(Particle2d* p)
{
  p->m_pos = NewPos(); // was: Vec3f(m_local[12], m_local[13], m_local[14]);
  p->m_time = NewTime();
}

void ParticleEffect2d::HandleDeadParticle(Particle2d* p)
{
  p->m_isDead = true;
}

void ParticleEffect2d::Update()
{
  if (m_isDead)
  {
    return;
  }

  if (m_tris.empty())
  {
    return;
  }

  float dt = TheTimer::Instance()->GetDt();
  bool isDead = true;

  int s = m_particles.size();
  Assert(m_tris.size() == (unsigned int)(2 * s));

  for (int i = 0; i < s; i++)
  {
    Particle2d& p = m_particles[i];

    // Update time - dead ?
    p.m_time += dt;
    if (p.m_time > m_maxTime)
    {
      HandleDeadParticle(&p);
      // TODO continue ??
    }
    isDead &= p.m_isDead;

    p.m_vel += p.m_acc * dt;
    p.m_pos += p.m_vel * dt; // TODO not accurate    
  }
  m_isDead = isDead;

  // TODO Not needed if we can disable Z-writing..?
  std::sort(m_particles.begin(), m_particles.end());

  Matrix mat;
  mat.ModelView(); // Get Modelview matrix
  mat = m_combined * mat; // also take all rotation into account
  Vec3f up(mat[1], mat[5], mat[9]);
  Vec3f right(mat[0], mat[4], mat[8]);

  for (int i = 0; i < s; i++)
  {
    Particle2d& p = m_particles[i];

    Vec3f v0 = p.m_pos + ( up + right) * m_size;
    Vec3f v1 = p.m_pos + ( up - right) * m_size;
    Vec3f v2 = p.m_pos + (-up - right) * m_size;
    Vec3f v3 = p.m_pos + (-up + right) * m_size;

    AmjuGL::Vert verts[4] = 
    {
      AmjuGL::Vert(v0.x, v0.y, v0.z,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
      AmjuGL::Vert(v1.x, v1.y, v1.z,   1, 1,   0, 1, 0),
      AmjuGL::Vert(v2.x, v2.y, v2.z,   0, 1,   0, 1, 0),
      AmjuGL::Vert(v3.x, v3.y, v3.z,   0, 0,   0, 1, 0)
    };

    AmjuGL::Tri* tri = &m_tris[i * 2];
    tri->m_verts[0] = verts[0];
    tri->m_verts[1] = verts[1];
    tri->m_verts[2] = verts[2];

    tri = &m_tris[i * 2 + 1];
    tri->m_verts[0] = verts[0];
    tri->m_verts[1] = verts[2];
    tri->m_verts[2] = verts[3];
  }
}

void ParticleEffect2d::Start()
{
  m_isDead = false;

  for (int i = 0; i < m_numParticles; i++)
  {
    Particle2d& p = m_particles[i];
    p.m_pos = NewPos();
    p.m_vel = NewVel();
    p.m_acc = NewAcc();
    p.m_time = NewTime();
  }
}
}
