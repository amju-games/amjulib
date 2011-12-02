#include "Player.h"
#include <AmjuGL.h>
#include <Teapot.h>
#include "Ve1SceneGraph.h"
#include <iostream>

namespace Amju
{
class PlayerSceneNode : public SceneNode
{
public:
  PlayerSceneNode(Player* p) : m_player(p)
  {
  }

  virtual void Draw()
  {
    AmjuGL::PushMatrix();
    Vec3f pos = m_player->GetPos();
    AmjuGL::Translate(pos.x, pos.y, pos.z);
    static Teapot tp;
    tp.Draw();
    AmjuGL::PopMatrix();
  }

protected:
  Player* m_player;
};

const char* Player::TYPENAME = "player";

bool Player::Init()
{
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  root->AddChild(new PlayerSceneNode(this));

  return true;
}

void Player::Update()
{
  // Move to new pos ?
//  float sqd = (GetPos() - m_newPos).SqLen();
  Vec3f vel = m_newPos - GetPos();
  SetVel(vel);
//std::cout << "Vel: " << m_vel.x << ", " << m_vel.y << ", " << m_vel.z << "\n";

  GameObject::Update();
}

}

