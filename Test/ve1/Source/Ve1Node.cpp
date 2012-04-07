#include "Ve1Node.h"
#include <iostream>
#include <SceneNode.h>
#include <DrawAABB.h>
#include <Unproject.h>
#include <GuiText.h>
#include "Ve1SceneGraph.h"
#include "Ve1Object.h"

namespace Amju
{
void Ve1Node::Draw()
{
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  PushColour();
  if (m_obj->IsSelected())
  {
    MultColour(Colour(1, 0, 0, 1));
  }
  else
  {
    MultColour(Colour(0.6, 0.6, 0.6, 1));
  }

  Vec2f screenpos;
  Vec3f pos = m_obj->GetPos();
  Project(pos, &screenpos);

  Assert(m_obj->GetAABB());
  DrawSolidAABB(*(m_obj->GetAABB()));
  DrawAABB(*(m_obj->GetAABB()));

  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);

  // TODO Draw info
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

  AmjuGL::PushMatrix();
  AmjuGL::SetIdentity();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::PushMatrix();
  AmjuGL::SetIdentity();

  MultColour(Colour(0, 0, 1, 1));
  GuiText text;
  text.SetTextSize(0.5f);
  std::string s = "Object " + ToString(m_obj->GetId()) + " " + m_obj->GetTypeName();
  text.SetText(s);
  text.SizeToText();
  text.SetJust(GuiText::AMJU_JUST_LEFT);
  text.SetPos(screenpos);
  text.Draw();

  PopColour();

  AmjuGL::PopMatrix();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::PopMatrix();

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_BLEND);
}

}


