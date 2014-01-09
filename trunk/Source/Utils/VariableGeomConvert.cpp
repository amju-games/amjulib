#include "VariableGeomConvert.h"

namespace Amju
{
Vec2f ToVec2f(const Variable& v)
{
  Assert(v.IsVectorType());
  const VariableVec& vec = v.GetVector();
  Assert(vec.size() == 2);
  Assert(vec[0].IsFloatType());
  Assert(vec[1].IsFloatType());
  return Vec2f(vec[0].GetFloat(), vec[1].GetFloat());
}

Vec3f ToVec3f(const Variable& v)
{
  Assert(v.IsVectorType());
  const VariableVec& vec = v.GetVector();
  Assert(vec.size() == 3);
  Assert(vec[0].IsFloatType());
  Assert(vec[1].IsFloatType());
  Assert(vec[2].IsFloatType());
  return Vec3f(vec[0].GetFloat(), vec[1].GetFloat(), vec[2].GetFloat());
}

Matrix ToMatrix(const Variable& v)
{
  Assert(v.IsVectorType());
  const VariableVec& vec = v.GetVector();
  Assert(vec.size() == 16);
  Matrix mtx;
  for (int i = 0; i < 16; i++)
  {
    Assert(vec[i].IsFloatType());
    mtx[i] = vec[i].GetFloat();
  }
  return mtx;
}

Variable ToVariable(const Vec2f& v2)
{
  VariableVec vvec;
  vvec.push_back(Variable(v2.x));
  vvec.push_back(Variable(v2.y));
  return vvec;
}

Variable ToVariable(const Vec3f& v3)
{
  VariableVec vvec;
  vvec.push_back(Variable(v3.x));
  vvec.push_back(Variable(v3.y));
  vvec.push_back(Variable(v3.z));
  return vvec;
}

Variable ToVariable(const Matrix& mat)
{
  VariableVec vvec;
  for (int i = 0; i < 16; i++)
  {
    vvec.push_back(Variable(mat[i]));
  }
  return vvec;
}
}

