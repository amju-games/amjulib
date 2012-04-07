#ifndef VE1_NODE_H_INCLUDED
#define VE1_NODE_H_INCLUDED

#include <SceneNode.h>

namespace Amju
{
class Ve1Object;

class Ve1Node : public SceneNode
{
public:
  Ve1Node(Ve1Object* obj) : m_obj(obj) { } 

  virtual void Draw();

protected:
  Ve1Object* m_obj;
};
}

#endif

