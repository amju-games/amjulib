#ifndef ROOM_NODE_H_INCLUDED
#define ROOM_NODE_H_INCLUDED

#include <AmjuGL.h>
#include <TriList.h>
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
  RCPtr<TriListStatic> m_triList;
  Room* m_room;
};
}

#endif
