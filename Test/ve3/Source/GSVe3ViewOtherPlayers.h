#ifndef GS_VE3VIEWOTHERPLAYERS_H_INCLUDED
#define GS_VE3VIEWOTHERPLAYERS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
#include "Ve1SpriteNode.h"

namespace Amju 
{
class Player;

class GSVe3ViewOtherPlayers : public GSGui
{
  GSVe3ViewOtherPlayers();
  friend class Singleton<GSVe3ViewOtherPlayers>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void NextPlayer();
  void PrevPlayer();

  void OnGiveFood();
  void OnGiveTreasure();
  void OnSeeGuestbook();

private:
  void SetPlayerSprite();

private:
  Ve1SpriteNode m_spriteNode;
  RCPtr<Player> m_player;
};
typedef Singleton<GSVe3ViewOtherPlayers> TheGSVe3ViewOtherPlayers;
} // namespace
#endif
