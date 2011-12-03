#include "GSMain.h"
#include <AmjuGL.h>
#include <Unproject.h>
#include <ClipLineSegBox.h>
#include <iostream>
#include <Game.h>
#include "Terrain.h"
#include "Ve1SceneGraph.h"
#include "LocalPlayer.h"
#include "Ve1OnlineReqManager.h"

namespace Amju
{
// TODO TEMP TEST
class MoveReq : public Ve1Req
{
public:
  MoveReq(const std::string& url, const Vec3f& pos) : Ve1Req(url, "Move req"), m_requestedPos(pos)
  {
  }

  virtual void HandleResult()
  {
    if (GetResult().GetSuccess())
    {
      std::cout << "Move req success! " << GetResult().GetString() << "\n";
    }
    else
    {
      // TODO Log those errors ?
      std::cout << "Move req failure! " << GetResult().GetErrorString() << "\n";
      //return;
    }

    // TODO Check for login timeout

    // Either wait till we get this, then send the local human player to the desired location;
    // OR, immediately move, (reducing lag) but go back if the request is then denied.

    // For now, wait till successful response then move

    // TODO Check for success/failure of request
std::cout << "Got response from server, moving local player to requested pos...\n";
    GetLocalPlayer()->MoveTo(m_requestedPos);
  }

private:
  Vec3f m_requestedPos;
};

GSMain::GSMain()
{
  m_moveRequest = false;


}

void GSMain::Update()
{
  GSBase::Update();

  // TODO Update all GOs
  //GetLocalPlayer()->Update();

  TheGame::Instance()->UpdateGameObjects();
}

void GSMain::DoMoveRequest()
{

    Vec3f mouseWorldNear;
    Vec3f mouseWorldFar;

    Unproject(m_mouseScreen, 0, &mouseWorldNear);
    Unproject(m_mouseScreen, 1, &mouseWorldFar);
    LineSeg lineSeg(mouseWorldNear, mouseWorldFar);


    // Draw for debugging
    std::cout << "Selecting, mouse: x: " << m_mouseScreen.x << " y: " << m_mouseScreen.y << "\n";
    AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D);
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    AmjuGL::DrawLine(AmjuGL::Vec3(mouseWorldNear.x + 0.1f, mouseWorldNear.y + 0.1f, mouseWorldNear.z),
      AmjuGL::Vec3(mouseWorldFar.x, mouseWorldFar.y, mouseWorldFar.z));
    AmjuGL::PopAttrib();

    GameObject* selectedObj = 0;
    GameObjects* objs = TheGame::Instance()->GetGameObjects();
    float bestDist = 999999.9f;
    for (GameObjects::iterator it = objs->begin(); it != objs->end(); ++it)
    {
      GameObject* pgo = it->second;
      Assert(pgo);
      AABB* aabb = pgo->GetAABB();
      if (aabb && Clip(lineSeg, *aabb, 0))
      {
        // Line seg intersects this box
        // Choose object whose centre (position) is closest to line seg..?
        float dist = LineSeg(mouseWorldNear, mouseWorldFar).SqDist(pgo->GetPos());
        //float dist = (mouseWorldNear - pgo->GetPos()).SqLen(); // pick closest
        if (dist < bestDist)
        {
          bestDist = dist;
          selectedObj = pgo;
        }
      }
    }
    if (selectedObj)
    {
      const std::string name = selectedObj->GetTypeName();
      std::cout << "Selected " << name << " ID: " << selectedObj->GetId() << "\n";
    }
    else
    {
      std::cout << "Ground clicked...\n";
      Vec3f pos = Terrain::GetTerrain()->GetMousePos(lineSeg);
      std::cout << "Pos: " << pos.x << ", " << pos.y << ", " << pos.z << "\n";

      TheOnlineReqManager::Instance()->AddReq(new MoveReq(MakeUrl(MOVE_REQ), pos), 1);
    }
}

void GSMain::Draw()
{
  GSBase::Draw();

  GetVe1SceneGraph()->Draw();

  if (m_moveRequest)
  {
    // Find point on ground clicked; create request and send to server
    DoMoveRequest();
    m_moveRequest = false;
  }
}

void GSMain::Draw2d()
{
}

void GSMain::OnActive()
{
  GSBase::OnActive();

  GetVe1SceneGraph()->Clear();
  GetVe1SceneGraph()->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);

  // Objects are loaded by ObjectManager and added to scene graph.

  // Load Player, add to scene graph 
  GetLocalPlayer()->Init();
}

bool GSMain::OnCursorEvent(const CursorEvent& ce)
{
  m_mouseScreen.x = ce.x;
  m_mouseScreen.y = ce.y;

  return false;
}

bool GSMain::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  // Player has clicked somewhere on screen.
  // Response will depend on whether an object was clicked, or a GUI element.
  // If the ground was clicked, make a request to move to that location.

  // GUI Elements: these have higher priority so if we get here we have clicked on a 
  // GameObject or the ground

  if (!mbe.isDown)
  {

    // Check for GOs: use code in amjuWW

    // Move to location:
    m_moveRequest = true;
  }

  return false;
}
} // namespace
