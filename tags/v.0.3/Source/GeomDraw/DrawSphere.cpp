#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "DrawSphere.h"
#include <Sphere.h>
#include <AmjuGL.h>

#ifdef AMJU_USE_GLUT
#ifdef WIN32
#include <GL/glut.h>
#endif // WIN32
#ifdef MACOSX
#include <GLUT/glut.h>
#endif // MACOSX
#endif // AMJU_USE_GLUT

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
#ifdef AMJU_USE_GLUT
  AmjuGL::PushMatrix();
  AmjuGL::Translate(s.GetCentre().x, s.GetCentre().y, s.GetCentre().z);
  glutWireSphere(s.GetRadius(), 20, 20);
  AmjuGL::PopMatrix();
#endif
}
}
