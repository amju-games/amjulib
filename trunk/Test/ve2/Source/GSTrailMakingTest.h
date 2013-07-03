#ifndef GS_TRAIL_MAKING_TEST_H_INCLUDED
#define GS_TRAIL_MAKING_TEST_H_INCLUDED

#include <vector>
#include <Vec2.h>
#include <StringUtils.h>
#include "GSCogTestBase.h"

namespace Amju
{
typedef Singleton<class GSTrailMakingTest> TheGSTrailMakingTest;

struct TrailDot
{
  TrailDot(const Vec2f& pos, const std::string& str) : m_pos(pos), m_str(str), m_clicked(false) {}

  void Draw();

  Vec2f m_pos;
  std::string m_str; // 1, A, 2, B... etc
  bool m_clicked;
};

class GSTrailMakingTest : public GSCogTestBase
{
public:
  virtual void OnActive();
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();

  virtual bool OnCursorEvent(const CursorEvent& ce);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent& mbe);

protected:
  typedef std::vector<TrailDot> Dots;
  Dots m_dots;
};
}

#endif
