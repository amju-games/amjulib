/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#include <math.h>
#include "KeyFrame.h"
#include "File.h"
#include "StringUtils.h"
#include "Lerp.h"

namespace Amju
{
KeyFrame::KeyFrame() : m_time(0)
{
}

float KeyFrame::GetTime() const
{
  return m_time;
}

const Matrix& KeyFrame::GetMatrix() const
{
  return m_matrix;
}

Matrix KeyFrame::Interpolate(const RST& r1, const RST& r2, float t)
{
  //Vec3 lerpScale = Lerp(r1.m_scale, r2.m_scale, t);

  Vec3f lerpTranslate = Lerp(r1.m_translate, r2.m_translate, t);

  Quaternion slerpRotate = Quaternion::Slerp(r1.m_quat, r2.m_quat, t);
  slerpRotate.Normalize();

#ifdef QUAT_SLERP_DEBUG
std::cout << " Quat slerp:\n\tQ1: " << ToString(r1.m_quat)
  << "\n\tQ2: " << ToString(r2.m_quat)
  << "\n\tSlerp: " << ToString(slerpRotate)
  << "\n";
#endif

  Matrix R, S, T;
  slerpRotate.CreateMatrix(&R);
  //S.Scale(lerpScale.m_x, lerpScale.m_y, lerpScale.m_z);
  T.Translate(lerpTranslate);

  //return R * S * T;
  return R * T; // no scale for now
}

RST CreateRST(const Matrix& mat)
{
  RST rst;
  rst.m_translate = Vec3f(mat[12], mat[13], mat[14]);
  rst.m_quat.CreateFromMatrix(mat);

  // Check that Quat is unit length
  //Assert(fabs(rst.m_quat.SquaredLength() - 1.0f) < 0.0001f);
  
  rst.m_quat.Normalize();

  // TODO No scale for now
  rst.m_scale = Vec3f(1.0f, 1.0f, 1.0f);
  return rst;
}

Matrix KeyFrame::Interpolate(
  PKeyFrame k1, PKeyFrame k2, float between)
{
  // TODO TEMP TEST - enable for no interpolation
  //return k1->m_matrix;

  // Create RST for each matrix, then interp.
  // - we can't directly interp 2 matrices.
  RST rst1 = CreateRST(k1->m_matrix);
  RST rst2 = CreateRST(k2->m_matrix);

  // Get a matrix of the interpolated RSTs
  return Interpolate(rst1, rst2, between);
}

bool KeyFrame::Load(File* pf)
{
  if (!pf->GetFloat(&m_time))
  {
    pf->ReportError("Expected keyframe time");
    return false;
  }
  if (m_time < 0)
  {
    pf->ReportError("Bad time for keyframe");
    return false;
  }

  return m_matrix.Load(pf);
}

#ifdef SCENE_EDITOR
bool KeyFrame::Save(File* pf)
{
  pf->WriteComment("// Keyfame time");
  pf->WriteFloat(m_time);
  pf->WriteComment("// Matrix");
  return m_matrix.Save(pf);
}
#endif

}

