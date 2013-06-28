#ifndef ROOM_NODE_H_INCLUDED
#define ROOM_NODE_H_INCLUDED

#include <AmjuGL.h>
#include <SceneNode.h>

namespace Amju
{
class Room;

class RoomNode : public SceneNode
{
public:
  RoomNode(Room*);
  virtual void Draw();
  void Build();

private:
  AmjuGL::Tris m_tris;
  Room* m_room;
};
}

#endif
