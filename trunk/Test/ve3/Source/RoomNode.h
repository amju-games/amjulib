#ifndef ROOM_NODE_H_INCLUDED
#define ROOM_NODE_H_INCLUDED

#include <AmjuGL.h>
#include <TriList.h>
#include <SceneNode.h>
#include "Tile.h"

namespace Amju
{
class Room;

class RoomNode : public SceneNode
{
public:
  RoomNode(const Tile& t);
  virtual void Draw();
  void Build();

private:
  RCPtr<TriListStatic> m_triList;
  Tile m_tile;
};
}

#endif
