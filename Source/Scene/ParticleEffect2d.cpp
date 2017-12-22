#include <AmjuFirst.h>
#include <Quaternion.h>
#include "ParticleEffect2d.h"
#include "File.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "LoadTextureResource.h"
#include <AmjuGL.h>
#include <AmjuFinal.h>

namespace Amju
{
const char* ParticleEffect2d::NAME = "particles";

Particle2d::Particle2d() : m_time(0), m_rot(0), m_rotVel(0), m_isDead(false)
{
  static int s_id = 0;
  m_id = s_id++;
}
  
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
  m_minY = 0;

  SetBlended(true);

  m_triList = (TriListDynamic*)AmjuGL::Create(TriListDynamic::DRAWABLE_TYPE_ID);
}

bool ParticleEffect2d::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected scene node name");
    return false;
  }

  m_texture = LoadTextureResource(f);
  Assert(m_texture);

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

void ParticleEffect2d::Set(
  const std::string& textureName, 
  float size, int num, float maxTime, float minY)
{
  m_texture = (Texture*)TheResourceManager::Instance()->GetRes(textureName);
  Assert(m_texture);
  m_size = size;
  m_numParticles = num;
  m_maxTime = maxTime;
  m_minY = minY;

  m_tris.resize(m_numParticles * 2);
  m_particles.resize(m_numParticles);
}

void ParticleEffect2d::Draw()
{
  if (m_isDead)
  {
    return;
  }

  // Update particles here, so we can get MView matrix.
  // TODO Move this to shader
  float dt = TheTimer::Instance()->GetDt();
  bool isDead = true;

  int s = m_particles.size();

  for (int i = 0; i < s; i++)
  {
    Particle2d& p = m_particles[i];

    UpdateParticle(&p, dt);

    // Update time - dead ?
    p.m_time += dt;
    if (p.m_time > m_maxTime)
    {
      HandleDeadParticle(&p);
      // TODO continue ??
    }

    if (p.m_pos.y < m_minY)
    {
      HandleMinY(&p);
    }

    isDead &= p.m_isDead;

    p.m_vel += p.m_acc * dt;
    p.m_pos += p.m_vel * dt; // TODO not accurate    
    p.m_rot += p.m_rotVel * dt;
  }
  m_isDead = isDead;

  Matrix mat;
  mat.ModelView(); // Get Modelview matrix
  mat = m_combined * mat; // also take all rotation into account
  Vec3f right(mat[0], mat[4], mat[8]);
  Vec3f up(mat[1], mat[5], mat[9]);
  Vec3f fwd(mat[2], mat[6], mat[10]);

  // Avoid reallocs
  //AmjuGL::Tris tris;
  m_tris.resize(s * 2);

  for (int i = 0; i < s; i++)
  {
    Particle2d& p = m_particles[i];

    Quaternion q(fwd, p.m_rot);
    Vec3f upR = q.RotateVec(up);
    Vec3f rightR = q.RotateVec(right);

    Vec3f v0 = p.m_pos + ( upR + rightR) * m_size;
    Vec3f v1 = p.m_pos + ( upR - rightR) * m_size;
    Vec3f v2 = p.m_pos + (-upR - rightR) * m_size;
    Vec3f v3 = p.m_pos + (-upR + rightR) * m_size;
    
    // TODO Also we can now set per-vertex colours!
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

  m_triList->Set(m_tris);

  // Do the actual Draw

  // TODO Also push lighting and blend state ?
  AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_combined); // NB combined
  m_texture->UseThisTexture();

  AmjuGL::Draw(m_triList);
 
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

float ParticleEffect2d::NewRot() const
{
  return 0;
}

float ParticleEffect2d::NewRotVel() const
{
  return 0;
}

void ParticleEffect2d::UpdateParticle(Particle2d* p, float dt)
{
  p->m_time += dt;
}
  
void ParticleEffect2d::Recycle(Particle2d* p)
{
  p->m_pos = NewPos(); 
  p->m_time = NewTime();
  // Shuuurely also:
  p->m_vel = NewVel();
  p->m_acc = NewAcc();
  p->m_rot = NewRot();
  p->m_rotVel = NewRotVel();
}

void ParticleEffect2d::HandleDeadParticle(Particle2d* p)
{
  p->m_isDead = true;
}

void ParticleEffect2d::Update()
{
}

void ParticleEffect2d::Start()
{
std::cout << "****ParticleEffect2d::Start!!!\n";

  m_isDead = false;

  for (int i = 0; i < m_numParticles; i++)
  {
    Particle2d& p = m_particles[i];
    p.m_pos = NewPos();
    p.m_vel = NewVel();
    p.m_acc = NewAcc();
    p.m_time = NewTime();
    p.m_rot = NewRot();
    p.m_rotVel = NewRotVel();
    p.m_isDead = false;
  }
}
}
