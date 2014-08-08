#ifndef GS_BASE_H
#define GS_BASE_H

#include <string>
#include <GameState.h>

namespace Amju
{
// Use this for render to texture etc
void DrawBunnyScene();

class GSBase : public GameState
{
public:
  GSBase();
  virtual void Update() override;
  virtual void Draw() override; // Set up proj and mview matrices
  virtual void Draw2d() override;
  virtual void OnActive() override;

  virtual bool OnKeyEvent(const KeyEvent&) override;

protected:
  float m_time;
  float m_maxTime;
  GSBase* m_nextState;
  std::string m_name;
  bool m_paused;
};
}

#endif
