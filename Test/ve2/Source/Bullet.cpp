#include "Bullet.h"
#include "GameConsts.h"
#include "SpriteNode.h"

namespace Amju
{
const char* Bullet::TYPENAME = "bullet";

static const float BULLET_SIZE = 5.0f; // TODO Different bullet types??!?

class BulletNode : public SpriteNode
{
public:
  BulletNode(Bullet* bullet) :
    SpriteNode("wh8.png", 1, 1, BULLET_SIZE, BULLET_SIZE), m_bullet(bullet) 
    {
    }

  virtual void Update()
  {
    m_aabb = *(m_bullet->GetAABB());
    SpriteNode::Update();
  }

private:
  Bullet* m_bullet;
};

Bullet::Bullet()
{
  // Set ID
  static int id = BULLET_START_ID;

std::cout << "New bullet: " << id << "\n";

  SetId(id++);
  BulletNode* bn = new BulletNode(this);
  SetSceneNode(bn);
  OnLocationEntry();
}

const char* Bullet::GetTypeName() const
{
  return TYPENAME;
}

void Bullet::Update()
{
  Ve1Object::Update();

  // Surely this is all done in base class ???
  if (m_sceneNode)
  {
    Matrix m;
    m.Translate(m_pos);
    m_sceneNode->SetLocalTransform(m);
  }

  if (GetAABB())
  {
    GetAABB()->Set(
      m_pos.x - BULLET_SIZE, m_pos.x + BULLET_SIZE,
      m_pos.y, m_pos.y + BULLET_SIZE,
      m_pos.z - BULLET_SIZE, m_pos.z + BULLET_SIZE);
  }
}

}
