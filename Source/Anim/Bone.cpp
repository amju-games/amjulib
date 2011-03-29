/*
Amju Games source code (c) Copyright Jason Colman 2000-2011
*/

#include "Bone.h"
#include "AmjuGL.h"
#include "File.h"

namespace Amju
{
/*
bool Vec3::Load(File* pf)
{
  if (!pf->GetFloat(&m_x))
  {
    pf->ReportError("Vec3: expected x");
    return false;
  }
  if (!pf->GetFloat(&m_y))
  {
    pf->ReportError("Vec3: expected y");
    return false;
  }
  if (!pf->GetFloat(&m_z))
  {
    pf->ReportError("Vec3: expected z");
    return false;
  }
  return true;
}

#ifdef SCENE_EDITOR
bool Vec3::Save(File* pf)
{
  pf->WriteComment("// Vec3 (x, y, z)");
  pf->WriteFloat(m_x);
  pf->WriteFloat(m_y);
  pf->WriteFloat(m_z);
  return true;
}
#endif

Vec3 operator+(const Vec3& w, const Vec3& v)
{
  return Vec3(w.m_x + v.m_x, w.m_y + v.m_y, w.m_z + v.m_z);
}

Vec3 operator-(const Vec3& w, const Vec3& v)
{
  return Vec3(w.m_x - v.m_x, w.m_y - v.m_y, w.m_z - v.m_z);
}

Vec3 operator*(const Vec3& v, float f)
{
  return Vec3(v.m_x * f, v.m_y * f, v.m_z * f);
}
*/

Bone::Bone()
{
  // TODO Do we need these ?
  m_transform.SetIdentity();
  m_combined.SetIdentity();
}

#ifdef _DEBUG

void Bone::Draw(Bone* pParent, int depth) const
{
  if (pParent)
  {
    // Draw bone using Combined Transforms
    AmjuGL::Vec3 v1(pParent->m_combined[12], pParent->m_combined[13], pParent->m_combined[14]);
    AmjuGL::Vec3 v2(m_combined[12], m_combined[13], m_combined[14]);

    AmjuGL::DrawLine(v1, v2);
  }

  // Draw children
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    m_children[i]->Draw(const_cast<Bone*>(this), depth + 1); // pass this as parent
  }
}
#endif

void Bone::AddChild(Bone* child)
{
  m_children.push_back(child);
}

void Bone::SetTransform(const Matrix& m)
{
  m_transform = m;
}

const Matrix& Bone::GetCombinedTransform() const
{
  return m_combined;
}

void Bone::CombineTransforms(Bone* parent)
{
  if (parent)
  {
    m_combined = m_transform * parent->m_combined;
  }
  else
  {
    m_combined = m_transform;
  }

  // Update children
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    m_children[i]->CombineTransforms(this);
  }
}

}

