#pragma once

#include <Cubemap.h>

namespace Amju
{
class CubemapOpenGL : public Cubemap
{
public:
  CubemapOpenGL() : m_texId(-1) {}

  bool Init() override;
  void Draw() override; // set up for shader to use

private:
  int m_texId;
};
}

