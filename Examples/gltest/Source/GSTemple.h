#pragma once

#include "GSBase.h"

namespace Amju
{
// Base class for "temple" demos, e.g. fire temple, water temple
class GSTemple : public GSBase
{
public:
  GSTemple() : m_loadedOk(false) {}
  void OnActive() override;
  void DrawScene() override;

protected:
  std::string m_levelFilename;
  bool m_loadedOk;

};
}

