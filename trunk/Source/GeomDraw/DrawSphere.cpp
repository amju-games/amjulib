#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "DrawSphere.h"
#include <Sphere.h>
#include <AmjuGL.h>
#ifdef WIN32
#ifdef _DEBUG
#include <GL/glut.h>
#endif
#endif

namespace Amju
{
void DrawCircleXY(float r)
{
  static const float STEP = 0.1f;
  for (float i = 0; i < 2* M_PI; i += STEP)
  {
  }
}

void DrawSphere(const Sphere& s)
{
#ifdef WIN32
#ifdef _DEBUG
  AmjuGL::PushMatrix();
  AmjuGL::Translate(s.GetCentre().x, s.GetCentre().y, s.GetCentre().z);
  glutWireSphere(s.GetRadius(), 5, 5);
  AmjuGL::PopMatrix();
#endif
#endif
}
}
