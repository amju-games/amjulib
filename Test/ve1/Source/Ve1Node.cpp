#include "Ve1Node.h"
#include <iostream>
#include <SceneNode.h>
#include <DrawAABB.h>
#include <Unproject.h>
#include <GuiText.h>
#include "Ve1SceneGraph.h"
#include "Ve1Object.h"
#include "Useful.h"

namespace Amju
{
void Ve1Node::Draw()
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  PushColour();
  if (m_obj->IsSelected())
  {
    MultColour(Colour(1, 0, 0, 1));
  }
  else
  {
    MultColour(Colour(0.6f, 0.6f, 0.6f, 1.0f));
  }

  Assert(m_obj->GetAABB());
  DrawSolidAABB(*(m_obj->GetAABB()));
  DrawAABB(*(m_obj->GetAABB()));

  Vec2f screenpos;
  Vec3f pos = m_obj->GetPos();
  Project(pos, &screenpos);

  PopColour();

  AmjuGL::PopAttrib();
}

void Ve1NameNode::Draw()
{
  // Print name 
  // TODO Do all these in one go, to minimise state changes
  AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND | AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

  GuiText text;
  text.SetTextSize(5.0f); // TODO CONFIG
  text.SetText(ToString(*m_obj));
    
  static const float MAX_NAME_WIDTH = 4.0f; // minimise this to reduce overdraw - calc from text
  text.SetSize(Vec2f(MAX_NAME_WIDTH, 1.0f));
  text.SetJust(GuiText::AMJU_JUST_CENTRE);
  //text.SetInverse(true);
  //text.SetDrawBg(true);

  AmjuGL::PushMatrix();
    
  Matrix m;
  m.SetIdentity();
  //Vec3f tr(m_local[12], m_local[13], m_local[14]);
  Vec3f tr(m_combined[12], m_combined[13], m_combined[14]);
  m.Translate(tr);
  AmjuGL::MultMatrix(m);
  static const float SCALE_FACTOR = 20.0f;
  float x = MAX_NAME_WIDTH * SCALE_FACTOR * -0.5f;
  AmjuGL::Translate(x, 60.0f, 0); // TODO CONFIG
    
  AmjuGL::Scale(SCALE_FACTOR, SCALE_FACTOR, 10);  

  text.Draw();
  AmjuGL::PopMatrix();
  AmjuGL::PopAttrib();

  /*
  AmjuGL::PushAttrib(
    AmjuGL::AMJU_TEXTURE_2D | AmjuGL:: AMJU_LIGHTING | AmjuGL::AMJU_BLEND | AmjuGL::AMJU_DEPTH_READ | AmjuGL::AMJU_DEPTH_WRITE);

  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);

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
  text.SetLocalPos(screenpos);
  text.Draw();


  AmjuGL::PopMatrix();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::PopMatrix();

  AmjuGL::PopAttrib();
  */
}

}


