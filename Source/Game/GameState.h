#ifndef GAME_STATE_H
#define GAME_STATE_H

namespace Amju
{
class GameState 
{
public:
  virtual ~GameState() {}

  virtual void Update() = 0;
  virtual void Draw() = 0;
  virtual void Draw2d() = 0;
  virtual void OnActive();
  virtual void OnDeactive();

protected:
};
}

#endif
