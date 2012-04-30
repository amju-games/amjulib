#include <iostream>
#include "ProtoObject.h"
#include <SceneNode.h>
#include <DrawAABB.h>
#include <Unproject.h>
#include <GuiText.h>
#include "Ve1SceneGraph.h"

namespace Amju
{
class ProtoSceneNode : public SceneNode
{
public:
  ProtoSceneNode(ProtoObject* po) : m_obj(po) {}

  virtual void Draw()
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
    text.SetLocalPos(screenpos);
    text.Draw();

    PopColour();

    AmjuGL::PopMatrix();
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::PopMatrix();

    AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
    AmjuGL::Disable(AmjuGL::AMJU_BLEND);
  }

private:
  ProtoObject* m_obj;
};

ProtoObject::ProtoObject()
{
//  static const float XSIZE = 1000.0f;
//  static const float YSIZE = 1000.0f;

  // Add node to Scene Graph
  ProtoSceneNode* psn = new ProtoSceneNode(this);
  SceneNode* root = GetVe1SceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);

/*
  psn->GetAABB()->Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y, m_pos.y + YSIZE,
    m_pos.z - XSIZE, m_pos.z + XSIZE);
*/

  root->AddChild(psn);
//  m_sceneNode = psn;
}

AABB* ProtoObject::GetAABB()
{
  return &m_aabb;
}

void ProtoObject::SetProtoLocation(int loc)
{
  m_location = loc;
}

void ProtoObject::Update()
{
  static const float XSIZE = 10.0f;
  static const float YSIZE = 20.0f;
  GetAABB()->Set(
    m_pos.x - XSIZE, m_pos.x + XSIZE,
    m_pos.y, m_pos.y + YSIZE,
    m_pos.z - XSIZE, m_pos.z + XSIZE);
  
}

const char* ProtoObject::GetTypeName() const
{
  return "PROTO";
}

void ProtoObject::OnLocationEntry()
{
  // ?
}

}

