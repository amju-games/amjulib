#pragma once

#include <DepthOfField.h>
#include <TriList.h>

namespace Amju
{
class DepthOfFieldES2 : public DepthOfField
{
public:
  void Draw() override;
  bool Init() override;
  
private:
  // Render target
  // TODO
  
  // Quad we render to back buffer
  RCPtr<TriListStatic> m_triList;

};
}

