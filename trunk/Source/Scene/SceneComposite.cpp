#include "SceneComposite.h"
#include "File.h"
#include "AmjuGL.h"

namespace Amju
{
//const char* SceneComposite::NAME = "comp";

void SceneComposite::Draw()
{
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);

  unsigned int s = m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    m_children[i]->Draw();
  }

  AmjuGL::PopMatrix();
}

void SceneComposite::Update()
{
  unsigned int s = m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    m_children[i]->Update();
  }
}

bool SceneComposite::Load(File* f)
{
  if (!SceneNode::Load(f))
  {
    return false;
  }

  int numChildren = 0;
  f->GetInteger(&numChildren);
  for (int i = 0; i < numChildren;  i++)
  {
    std::string s;
    f->GetDataLine(&s); // TODO Change this fn name ?
    PSceneNode p = TheSceneNodeFactory::Instance()->Create(s);
    Assert(p);
    p->Load(f);
    m_children.push_back(p);
  }
  return true;
}

void SceneComposite::CombineTransform()
{
  SceneNode::CombineTransform();

  unsigned int s = m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    m_children[i]->CombineTransform();
  }
}

void SceneComposite::UpdateBoundingVol()
{
  // TODO Calc updated bounding vol for this node
  // Our bounding box must contain the bounding box of all children.

  if (m_parent)
  {
    m_parent->UpdateBoundingVol();
  }
}

void SceneComposite::AddChild(PSceneNode node)
{
  m_children.push_back(node);
}

}
