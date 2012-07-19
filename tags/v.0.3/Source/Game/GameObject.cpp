#include "GameObject.h"
#include "Timer.h"
#include "File.h"

namespace Amju
{
GameObject::GameObject()
{
  m_id = -1;
}

void GameObject::Update()
{
  float dt = TheTimer::Instance()->GetDt();
  Vec3f old = m_vel;
  m_vel += m_acc * dt;
  Vec3f tr = (old + m_vel) * (dt * 0.5f); 
  m_pos += tr;
}

void GameObject::SetPos(const Vec3f& v)
{
  m_pos = v;
}

void GameObject::SetVel(const Vec3f& v)
{
  m_vel = v;
}

void GameObject::SetAcc(const Vec3f& v)
{
  m_acc = v;
}

const Vec3f& GameObject::GetPos() const
{
  return m_pos;
}

const Vec3f& GameObject::GetVel() const
{
  return m_vel;
}

const Vec3f& GameObject::GetAcc() const
{
  return m_acc;
}

bool GameObject::Load(File* f)
{
  if (!f->GetInteger(&m_id))
  {
    return false;
  }
  return true;
}
}
