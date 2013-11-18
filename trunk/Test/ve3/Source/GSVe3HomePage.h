#ifndef GS_VE3HOMEPAGE_H_INCLUDED
#define GS_VE3HOMEPAGE_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
#include "Ve1SpriteNode.h"

namespace Amju 
{
class GSVe3HomePage : public GSGui
{
  GSVe3HomePage();
  friend class Singleton<GSVe3HomePage>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

protected:
  Ve1SpriteNode m_spriteNode;
};
typedef Singleton<GSVe3HomePage> TheGSVe3HomePage;
} // namespace
#endif
