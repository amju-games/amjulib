#pragma once

#include <DepthOfField.h>

namespace Amju
{
class DofOpenGL : public DepthOfField
{
public:
  void Draw() override;
  bool Init() override;
};
}

