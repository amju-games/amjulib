/*
Amju Games source code (c) Copyright Jason Colman 2005-2009
*/

#include "AmjuFirst.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Common.h"
#include "File.h"
#include "StringUtils.h"
#include "AmjuFinal.h"

namespace Amju
{
std::string ToString(const Quaternion& q)
{
  std::string s = "w: ";
  s += ToString(q.m_w, 2);
  s += " x: ";
  s += ToString(q.m_x, 2);
  s += " y: ";
  s += ToString(q.m_y, 2);
  s += " z: ";
  s += ToString(q.m_z, 2);
  return s;
}

Quaternion::Quaternion()
{
  AMJU_CALL_STACK;

  m_x = m_y = m_z = 0.0f;
  m_w = 1.0f;
}

Quaternion::Quaternion(float w, float x, float y, float z)
{
  AMJU_CALL_STACK;

  m_w = w;
  m_x = x;
  m_y = y;
  m_z = z;
}

Quaternion::Quaternion(float w, const Vec3f& v)
{
  AMJU_CALL_STACK;

  m_w = w;
  m_x = v.x;
  m_y = v.y;
  m_z = v.z;
}

void Quaternion::SetIdentity()
{
  AMJU_CALL_STACK;

  m_x = m_y = m_z = 0.0f;
  m_w = 1.0f;
}

float Quaternion::SquaredLength() const
{
  return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

void Quaternion::Normalize()
{
  float n = 1.0f / sqrt(SquaredLength());
  m_w *= n;
  m_x *= n;
  m_y *= n;
  m_z *= n;
}

bool Quaternion::Load(File* pf)
{
  AMJU_CALL_STACK;

  if (!pf->GetFloat(&m_w))
  {
    pf->ReportError("Quat: expected w");
    return false;
  }
  if (!pf->GetFloat(&m_x))
  {
    pf->ReportError("Quat: expected x");
    return false;
  }
  if (!pf->GetFloat(&m_y))
  {
    pf->ReportError("Quat: expected y");
    return false;
  }
  if (!pf->GetFloat(&m_z))
  {
    pf->ReportError("Quat: expected z");
    return false;
  }
  return true;
}

#ifdef SCENE_EDITOR
bool Quaternion::Save(File* pf)
{
  AMJU_CALL_STACK;

  pf->WriteComment("// Quat (w, x, y, z)");
  pf->WriteFloat(m_w);
  pf->WriteFloat(m_x);
  pf->WriteFloat(m_y);
  pf->WriteFloat(m_z);
  return true;
}
#endif

void Quaternion::CreateFromAxisAngle(float x, float y, float z, float degrees)
{
  AMJU_CALL_STACK;

    // First we want to convert the degrees to radians 
    // since the angle is assumed to be in radians
    float angle = float((degrees / 180.0f) * pi);
    // Here we calculate the sin( theta / 2) once for optimization
    float result = (float)sin( angle / 2.0f );
		
    // Calcualte the w value by cos( theta / 2 )
    m_w = (float)cos( angle / 2.0f );
    // Calculate the x, y and z of the quaternion
    m_x = float(x * result);
    m_y = float(y * result);
    m_z = float(z * result);
}

void Quaternion::CreateFromMatrix(const Matrix& m)
{
    //http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

  /* original from web page:

  float trace = a[0][0] + a[1][1] + a[2][2] + 1.0f;
  if( trace > M_EPSILON ) {
    float s = 0.5f / sqrtf(trace);
    q.w = 0.25f / s;
    q.x = ( a[2][1] - a[1][2] ) * s;
    q.y = ( a[0][2] - a[2][0] ) * s;
    q.z = ( a[1][0] - a[0][1] ) * s;
  } else {
    if ( a[0][0] > a[1][1] && a[0][0] > a[2][2] ) {
      float s = 2.0f * sqrtf( 1.0f + a[0][0] - a[1][1] - a[2][2]);
      q.w = (a[1][2] - a[2][1] ) / s;
      q.x = 0.25f * s;
      q.y = (a[0][1] + a[1][0] ) / s;
      q.z = (a[0][2] + a[2][0] ) / s;
    } else if (a[1][1] > a[2][2]) {
      float s = 2.0f * sqrtf( 1.0f + a[1][1] - a[0][0] - a[2][2]);
      q.w = (a[0][2] - a[2][0] ) / s;
      q.x = (a[0][1] + a[1][0] ) / s;
      q.y = 0.25f * s;
      q.z = (a[1][2] + a[2][1] ) / s;
    } else {
      float s = 2.0f * sqrtf( 1.0f + a[2][2] - a[0][0] - a[1][1] );
      q.w = (a[0][1] - a[1][0] ) / s;
      q.x = (a[0][2] + a[2][0] ) / s;
      q.y = (a[1][2] + a[2][1] ) / s;
      q.z = 0.25f * s;
    }
  }
  */

  /* 
     
	3 * 3 matrix

    [0][0] [0][1] [0][2]
    [1][0] [1][1] [1][2]
    [2][0] [2][1] [2][2]

    OpenGL 3 * 4 matrix equivalent:

    [0]    [4]    [8]
    [1]    [5]    [9]
    [2]    [6]    [10]

  */


  float trace = m[0] + m[5] + m[10] + 1.0f;
  static const float EPSILON = 0.0001f;
  if( trace > EPSILON ) 
  {
    float s = 0.5f / sqrtf(trace);
    m_w = 0.25f / s;
    m_x = (m[6] - m[9]) * s;
    m_y = (m[8] - m[2]) * s;
    m_z = (m[1] - m[4]) * s;
  } 
  else 
  {
    if ( m[0] > m[5] && m[0] > m[10] ) 
	{
      float s = 2.0f * sqrtf( 1.0f + m[0] - m[5] - m[0]);
      m_w = (m[9] - m[6]) / s;
      m_x = 0.25f * s;
      m_y = (m[4] + m[1]) / s;
      m_z = (m[8] + m[2]) / s;
    } 
	else if (m[5] > m[10]) 
	{
      float s = 2.0f * sqrtf( 1.0f + m[5] - m[0] - m[10]);
      m_w = (m[8] - m[2]) / s;
      m_x = (m[4] + m[1]) / s;
      m_y = 0.25f * s;
      m_z = (m[9] + m[6]) / s;
    } 
	else 
	{
      float s = 2.0f * sqrtf( 1.0f + m[10] - m[0] - m[5]);
      m_w = (m[4] - m[1]) / s;
      m_x = (m[8] + m[2]) / s;
      m_y = (m[9] + m[6]) / s;
      m_z = 0.25f * s;
    }
  }
}

void Quaternion::CreateMatrix(Matrix *pMatrix) const
{
  AMJU_CALL_STACK;

  CreateMatrix(pMatrix->m_e); // OK - we are a friend
}

void Quaternion::CreateMatrix(float *pMatrix) const
{
  AMJU_CALL_STACK;

    // Make sure the matrix has allocated memory to store the rotation data
    if(!pMatrix) return;
    // First row
    pMatrix[ 0] = 1.0f - 2.0f * ( m_y * m_y + m_z * m_z ); 
    pMatrix[ 1] = 2.0f * (m_x * m_y + m_z * m_w);
    pMatrix[ 2] = 2.0f * (m_x * m_z - m_y * m_w);
    pMatrix[ 3] = 0.0f;  
    // Second row
    pMatrix[ 4] = 2.0f * ( m_x * m_y - m_z * m_w );  
    pMatrix[ 5] = 1.0f - 2.0f * ( m_x * m_x + m_z * m_z ); 
    pMatrix[ 6] = 2.0f * (m_z * m_y + m_x * m_w );  
    pMatrix[ 7] = 0.0f;  
    // Third row
    pMatrix[ 8] = 2.0f * ( m_x * m_z + m_y * m_w );
    pMatrix[ 9] = 2.0f * ( m_y * m_z - m_x * m_w );
    pMatrix[10] = 1.0f - 2.0f * ( m_x * m_x + m_y * m_y );  
    pMatrix[11] = 0.0f;  
    // Fourth row
    pMatrix[12] = 0;  
    pMatrix[13] = 0;  
    pMatrix[14] = 0;  
    pMatrix[15] = 1.0f;
    // Now pMatrix[] is a 4x4 homogeneous matrix that can be applied to an OpenGL Matrix
}

Quaternion Quaternion::operator *(const Quaternion& q) const
{
  AMJU_CALL_STACK;

    Quaternion r;
    r.m_w = m_w*q.m_w - m_x*q.m_x - m_y*q.m_y - m_z*q.m_z;
    r.m_x = m_w*q.m_x + m_x*q.m_w + m_y*q.m_z - m_z*q.m_y;
    r.m_y = m_w*q.m_y + m_y*q.m_w + m_z*q.m_x - m_x*q.m_z;
    r.m_z = m_w*q.m_z + m_z*q.m_w + m_x*q.m_y - m_y*q.m_x;
	
    return(r);
}

Quaternion Quaternion::Slerp(
  const Quaternion& from, const Quaternion& to, float t)
{
  AMJU_CALL_STACK;

  // Stolen from www.gamasutra.com/features/19980703/quaternions_01.htm

  float to1[4];
  double omega, cosom, sinom, scale0, scale1;

  // calc cosine
  cosom = from.m_x * to.m_x + from.m_y * to.m_y + from.m_z * to.m_z
			       + from.m_w * to.m_w;

  // adjust signs (if necessary)
  if (cosom < 0)
  { 
    cosom = -cosom; 
    to1[0] = - to.m_x;
    to1[1] = - to.m_y;
    to1[2] = - to.m_z;
    to1[3] = - to.m_w;
  } 
  else  
  {
    to1[0] = to.m_x;
    to1[1] = to.m_y;
    to1[2] = to.m_z;
    to1[3] = to.m_w;
  }

  // calculate coefficients

  static const double DELTA = 0.001;
  if ( (1.0 - cosom) > DELTA ) 
  {
    // standard case (slerp)
    omega = acos(cosom);
    sinom = sin(omega);
    scale0 = sin((1.0 - t) * omega) / sinom;
    scale1 = sin(t * omega) / sinom;
  } 
  else 
  {        
    // "from" and "to" quaternions are very close 
    //  ... so we can do a linear interpolation
    scale0 = 1.0 - t;
    scale1 = t;
  }

  Quaternion res;

  // calculate final values
  res.m_x = (float)(scale0 * from.m_x + scale1 * to1[0]);
  res.m_y = (float)(scale0 * from.m_y + scale1 * to1[1]);
  res.m_z = (float)(scale0 * from.m_z + scale1 * to1[2]);
  res.m_w = (float)(scale0 * from.m_w + scale1 * to1[3]);

  return res;
}

Quaternion Quaternion::Conjugate() const
{
  return Quaternion(m_w, -m_x, -m_y, -m_z);
}

Vec3f Quaternion::RotateVec(const Vec3f& v) const
{
  // v' = q v q*
  Quaternion qv(0, v);
  Quaternion result((*this) * qv * Conjugate());
  result.Normalize();
  return Vec3f(result.m_x, result.m_y, result.m_z);
}
}

