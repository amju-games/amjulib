#include <utility>
#include "TextMaker.h"
#include "SceneMesh.h"
#include "CollisionMesh.h"
#include "AABB.h"

namespace Amju
{
void TextMaker::SetDataDir(const std::string& dataDir)
{
  m_dataDir = dataDir;
}

SceneNode* TextMaker::MakeChar(char k, float* width)
{
  if (k == ' ')
  {
    *width = 0.5f;
    return 0;
  }
  char c = tolower(k);

  std::string filename = m_dataDir + "_" + std::string(1, c) + "_.obj";
  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(filename);
  if (!mesh)
  {
    return 0;
  }

  SceneMesh* meshNode = new SceneMesh;
  meshNode->SetMesh(mesh);

  // Calc width - TODO Use const array, this is stupidly time consuming
  typedef std::map<char, AABB> WidthMap;
  static WidthMap wm;
  WidthMap::iterator it = wm.find(k);
  if (it == wm.end())
  {
    CollisionMesh cm;
    mesh->CalcCollisionMesh(&cm);
    AABB aabb;
    cm.CalcAABB(&aabb);
    *width = aabb.GetXSize();
    *(meshNode->GetAABB()) = aabb;
    wm[k] = aabb;
  }
  else
  {
    *(meshNode->GetAABB()) = it->second;
    *width = it->second.GetXSize();
  }

  return meshNode;
}

SceneNode* TextMaker::MakeText(const std::string& text)
{
  SceneNode* comp = new SceneNode;
  float xmax = 0;
  float x = 0;
  float y = 1.0f;
  for (unsigned int i = 0; i < text.size(); i++)
  {
    char c = text[i];
    if (c == '\n')
    {
      x = 0;
      y += 2.0f; // TODO CONFIG
      continue;
    }
    float w = 0;
    SceneNode* node = MakeChar(c, &w);
    if (node)
    {
      Matrix m;
      m.Translate(Vec3f(x, 0, y));
      node->SetLocalTransform(m);
      node->GetAABB()->Translate(Vec3f(x, 0, y));

      // Decorate node, e.g. colours, movement
      // (Decorator node must use bounding vol of node)
      comp->AddChild(Decorate(node));
    }
    x += w;
    if (x > xmax)
    {
      xmax = x;
    }
  }
  // Centre TODO make optional; centre each line!
  Matrix m;
  m.Translate(Vec3f(xmax * -0.5f, 0, y * -0.5f));
  comp->SetLocalTransform(m);

  // Makes this AABB union of child AABBs, need better name.
  // Also make this recursive 
  comp->UpdateBoundingVol();

  // Do this after comp AABB is properly set
  comp->RecursivelyTransformAABB(m);

  return comp;
}
}
