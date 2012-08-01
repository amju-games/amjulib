#ifndef VE1_NODE_H_INCLUDED
#define VE1_NODE_H_INCLUDED

#include <SceneNode.h>
#include "Ve1Object.h"

namespace Amju
{
class Ve1Node : public SceneNode
{
public:
  Ve1Node(Ve1Object* obj) : m_obj(obj) {} 

  virtual void Draw();

protected:
  RCPtr<Ve1Object> m_obj;
};

class Ve1NameNode : public SceneNode
{
public:
  Ve1NameNode(Ve1Object* obj) : m_obj(obj) {}
  virtual void Draw();

protected:
  RCPtr<Ve1Object> m_obj;
};
}

#endif

