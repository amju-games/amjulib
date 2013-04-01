#include <AmjuFirst.h>
#include <AmjuGL.h>
#include "OutlinerOpenGL.h"
#include "OpenGL.h"
#include <AmjuFinal.h>

namespace Amju
{
bool OutlinerOpenGL::Init()
{
  return true;
}

void OutlinerOpenGL::Draw()
{
  Assert(m_drawFunc);

  // Draw with lines, back faces, then draw normally

  glPolygonMode(GL_FRONT, GL_LINES);
  
  glLineWidth(10.0f);
  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING | AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  
  // TODO draw back faces
  m_drawFunc();

  AmjuGL::PopAttrib();

  // TODO Switch to drawing front faces
  m_drawFunc();
}

}

