#ifndef GS_OBB2_H
#define GS_OBB2_H

#include <vector>
#include <OBB2.h>
#include <Singleton.h>
#include "GSBase.h"

namespace Amju
{
class GSObb2Test : public GSBase
{
  GSObb2Test();
  friend class Singleton<GSObb2Test>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

private:
  typedef std::vector<OBB2> Obbs;
  Obbs m_obbs;
};

typedef Singleton<GSObb2Test> TheGSObb2Test;
}

#endif
