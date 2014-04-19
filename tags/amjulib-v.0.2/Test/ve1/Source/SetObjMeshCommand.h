#ifndef SET_OBJ_MESH_COMMAND_H_INCLUDED
#define SET_OBJ_MESH_COMMAND_H_INCLUDED

#include <Game.h>
#include <GuiElement.h>
#include "GSObjMesh.h"

namespace Amju
{
class SetObjMeshCommand : public GuiCommand
{
public:
  SetObjMeshCommand(int objId) : m_id(objId) {}
  virtual bool Do()
  {
    TheGSObjMesh::Instance()->SetId(m_id);
    TheGame::Instance()->SetCurrentState(TheGSObjMesh::Instance());
    return false;
  }

private:
  int m_id;
};

}

#endif

