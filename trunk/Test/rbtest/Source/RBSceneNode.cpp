#include "RBSceneNode.h"

namespace Amju
{
void RBSceneNode::Update()
{
  Assert(m_rb);
  m_rb->Update();
}

void RBSceneNode::Draw()
{
  Assert(m_rb);
  m_rb->Draw();
}

}


