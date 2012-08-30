/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#include "AmjuFirst.h"
#include "Frustum.h"
#include "Sphere.h"
#include "AABB.h"
#include "AmjuGL.h"
#include "AmjuFinal.h"

namespace Amju
{
Frustum::Frustum()
{
  for (int i = 0; i < 6; i++)
  {
    frustum[i][0] = 0;
    frustum[i][1] = 0;
    frustum[i][2] = 0;
    frustum[i][3] = 0;
  }
}

Frustum::FrustumResult Frustum::SphereInFrustum( float x, float y, float z, float radius ) const
{
  AMJU_CALL_STACK;

  // from http://www.lighthouse3d.com/opengl/viewfrustum/index.php?gatest

  FrustumResult res = AMJU_INSIDE;
  for (int p = 0; p < 6; p++)
  {
    float d = frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3];
    if (d <= -radius)
    {
      return AMJU_OUTSIDE;
    }
    else if (d < radius)
    {
      res = AMJU_PART_INSIDE;
    }
  }
  return res;
}

bool Frustum::Intersects(const Vec3f& v) const
{
  AMJU_CALL_STACK;

  for (int p = 0; p < 6; p++)
  {
    float d = frustum[p][0] * v.x + frustum[p][1] * v.y + frustum[p][2] * v.z + frustum[p][3];
    if (d < 0)
    {
      return false;
    }
  }
  return true;
}

Frustum::FrustumResult Frustum::Intersects(const AABB& aabb) const
{
  // http://www.lighthouse3d.com/opengl/viewfrustum/index.php?gatest3

  FrustumResult res = AMJU_INSIDE;
  for (int p = 0; p < 6; p++)
  {
    // Get "positive" and "negative" vertices for box for this plane
    Vec3f pos(aabb.GetMin(0), aabb.GetMin(1), aabb.GetMin(2));
    Vec3f neg(aabb.GetMax(0), aabb.GetMax(1), aabb.GetMax(2));

    if (frustum[p][0] >= 0)
    {
      pos.x = aabb.GetMax(0);
      neg.x = aabb.GetMin(0);
    }
    if (frustum[p][1] >= 0)
    {
      pos.y = aabb.GetMax(1);
      neg.y = aabb.GetMin(1);
    }
    if (frustum[p][2] >= 0)
    {
      pos.z = aabb.GetMax(2);
      neg.z = aabb.GetMin(2);
    }

    float distP = frustum[p][0] * pos.x + frustum[p][1] * pos.y + frustum[p][2] * pos.z + frustum[p][3];
    if (distP < 0)
    {
      return AMJU_OUTSIDE;
    }

    float distN = frustum[p][0] * neg.x + frustum[p][1] * neg.y + frustum[p][2] * neg.z + frustum[p][3];
    if (distN < 0)
    {
      res = AMJU_PART_INSIDE;
    }
  }

  return res;
}

/*
float Frustum::DistanceToPoint(const Vec3f& v) const
{
  AMJU_CALL_STACK;

  for (int p = 0; p < 6; p++)
  {
    float d = frustum[p][0] * v.x + frustum[p][1] * v.y + frustum[p][2] * v.z + frustum[p][3];
    if (d < 0)
    {
      return d;
    }
  }
  return 0;
}
*/

void Frustum::Create()
{
  AMJU_CALL_STACK;

  float	proj[16]; // For Grabbing The PROJECTION Matrix
  float	modl[16]; // For Grabbing The MODELVIEW Matrix
  float	clip[16]; // Result Of Concatenating PROJECTION and MODELVIEW
  float	t; 

  AmjuGL::GetMatrix(AmjuGL::AMJU_PROJECTION_MATRIX, proj);
  AmjuGL::GetMatrix(AmjuGL::AMJU_MODELVIEW_MATRIX, modl);
  
  // Concatenate (Multiply) The Two Matricies
  clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
  clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
  clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
  clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

  clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
  clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
  clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
  clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

  clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
  clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
  clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
  clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

  clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
  clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
  clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
  clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];


  // Extract the RIGHT clipping plane
  frustum[0][0] = clip[ 3] - clip[ 0];
  frustum[0][1] = clip[ 7] - clip[ 4];
  frustum[0][2] = clip[11] - clip[ 8];
  frustum[0][3] = clip[15] - clip[12];

  // Normalize it
  t = (float) sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
  frustum[0][0] /= t;
  frustum[0][1] /= t;
  frustum[0][2] /= t;
  frustum[0][3] /= t;


  // Extract the LEFT clipping plane
  frustum[1][0] = clip[ 3] + clip[ 0];
  frustum[1][1] = clip[ 7] + clip[ 4];
  frustum[1][2] = clip[11] + clip[ 8];
  frustum[1][3] = clip[15] + clip[12];

  // Normalize it
  t = (float) sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
  frustum[1][0] /= t;
  frustum[1][1] /= t;
  frustum[1][2] /= t;
  frustum[1][3] /= t;


  // Extract the BOTTOM clipping plane
  frustum[2][0] = clip[ 3] + clip[ 1];
  frustum[2][1] = clip[ 7] + clip[ 5];
  frustum[2][2] = clip[11] + clip[ 9];
  frustum[2][3] = clip[15] + clip[13];

  // Normalize it
  t = (float) sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
  frustum[2][0] /= t;
  frustum[2][1] /= t;
  frustum[2][2] /= t;
  frustum[2][3] /= t;


  // Extract the TOP clipping plane
  frustum[3][0] = clip[ 3] - clip[ 1];
  frustum[3][1] = clip[ 7] - clip[ 5];
  frustum[3][2] = clip[11] - clip[ 9];
  frustum[3][3] = clip[15] - clip[13];

  // Normalize it
  t = (float) sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
  frustum[3][0] /= t;
  frustum[3][1] /= t;
  frustum[3][2] /= t;
  frustum[3][3] /= t;


  // Extract the FAR clipping plane
  frustum[4][0] = clip[ 3] - clip[ 2];
  frustum[4][1] = clip[ 7] - clip[ 6];
  frustum[4][2] = clip[11] - clip[10];
  frustum[4][3] = clip[15] - clip[14];

  // Normalize it
  t = (float) sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
  frustum[4][0] /= t;
  frustum[4][1] /= t;
  frustum[4][2] /= t;
  frustum[4][3] /= t;


  // Extract the NEAR clipping plane.  This is last on purpose (see pointinfrustum() for reason)
  frustum[5][0] = clip[ 3] + clip[ 2];
  frustum[5][1] = clip[ 7] + clip[ 6];
  frustum[5][2] = clip[11] + clip[10];
  frustum[5][3] = clip[15] + clip[14];

  // Normalize it
  t = (float) sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
  frustum[5][0] /= t;
  frustum[5][1] /= t;
  frustum[5][2] /= t;
  frustum[5][3] /= t;
}

Frustum::FrustumResult Frustum::Intersects(const Sphere& bs) const
{
  AMJU_CALL_STACK;

  return SphereInFrustum(bs.GetCentre().x, 
                         bs.GetCentre().y, 
                         bs.GetCentre().z, 
                         bs.GetRadius());
}
}
