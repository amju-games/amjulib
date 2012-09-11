#ifndef AMJUGL_WINDOW_INFO_H
#define AMJUGL_WINDOW_INFO_H

#include <string>

namespace Amju
{
class AmjuGLWindowInfo
{
public:
  AmjuGLWindowInfo(int w, int h, bool full, const std::string& title);
  int GetWidth() const;
  int GetHeight() const;
  bool IsFullScreen() const;
  const char* GetTitle() const;

private:
  int m_width;
  int m_height;
  bool m_isFullScreen;
  std::string m_title;
};
}

#endif
