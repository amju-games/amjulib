#ifndef AMJUGL_WINDOW_INFO_H
#define AMJUGL_WINDOW_INFO_H

namespace Amju
{
class AmjuGLWindowInfo
{
public:
  AmjuGLWindowInfo(int w, int h, bool full);
  int GetWidth() const;
  int GetHeight() const;
  bool IsFullScreen() const;

private:
  int m_width;
  int m_height;
  bool m_isFullScreen;
};
}

#endif
