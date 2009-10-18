#include "AmjuGLWindowInfo.h"

namespace Amju
{
AmjuGLWindowInfo::AmjuGLWindowInfo(int w, int h, bool full) :
  m_width(w), m_height(h), m_isFullScreen(full)
{
}

int AmjuGLWindowInfo::GetWidth() const
{
  return m_width;
}

int AmjuGLWindowInfo::GetHeight() const
{
  return m_height;
}

bool AmjuGLWindowInfo::IsFullScreen() const
{
  return m_isFullScreen;
}
}
