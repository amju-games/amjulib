#pragma once

#include "Drawable.h"

namespace Amju
{
class Cubemap : public Drawable
{
public:
  enum { DRAWABLE_TYPE_ID = 6 };
  
  Cubemap() : m_textureUnitId(0) {}

  void SetTextureNames(const std::string str[6])
  {
    for (int i = 0; i < 6; i++) m_textureNames[i] = str[i];
  }

  void SetTextureUnitId(int t) { m_textureUnitId = t; }

protected:
  std::string m_textureNames[6];
  int m_textureUnitId;
};
}

