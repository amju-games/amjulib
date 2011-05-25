#ifndef GS_OBB2_H
#define GS_OBB2_H

#include <GameState.h>
#include <vector>
#include <OBB2.h>

namespace Amju
{
class GSObb2Test : public GameState
{
public:
  static const char* NAME;

  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

private:
  typedef std::vector<OBB2> Obbs;
  Obbs m_obbs;
};
}

#endif
