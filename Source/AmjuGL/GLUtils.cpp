#include "AmjuGL.h"
#include "GLUtils.h"

namespace Amju
{
void MultFrustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
  // Column major matrix
  float m[16];
  // See man glFrustum :-)
  m[0] = 2.0f * zNear / (right - left);
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;

  m[4] = 0;
  m[5] = 2 * zNear / (top - bottom);
  m[6] = 0;
  m[7] = 0;

  m[8] = (right + left) / (right - left);
  m[9] = (top + bottom) / (top - bottom);
  m[10] = - (zFar + zNear) / (zFar - zNear);
  m[11] = -1;

  m[12] = 0;
  m[13] = 0;
  m[14] = - (2.0f * zFar * zNear) / (zFar - zNear);
  m[15] = 0;

  AmjuGL::MultMatrix(m);
}

}

