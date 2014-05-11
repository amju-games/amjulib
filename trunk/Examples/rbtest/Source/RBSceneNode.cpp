#include <AmjuGL.h>
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
  AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  Assert(m_rb);
  m_rb->Draw();

  AmjuGL::PopAttrib();
}

}


