#include "RBBox.h"
#include <DrawOBB3.h>

namespace Amju
{
RBBox::RBBox()
{
  m_obb.SetExtents(Vec3f(2, 1, 1));
}

void RBBox::Draw()
{
  DrawOBB3(m_obb);
}

void RBBox::Update()
{
  RigidBody::Update();
  m_obb.SetOrientation(m_quat);
  m_obb.SetCentre(m_pos);
}
}
