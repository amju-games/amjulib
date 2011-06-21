#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "RCPtr.h"
#include "AmjuTypes.h"
#include "Matrix.h"
#include "Factory.h"
#include "Singleton.h"
#include "AABB.h"
#include "Colour.h"

namespace Amju
{
class File;
class SceneNode;
typedef RCPtr<SceneNode> PSceneNode;
class SceneGraph;

// This Scene Graph is based on the design in "Game Coding Complete".
// This base class has children - not Composite pattern.
// We can instantiate the base class to use as a container of children.
class SceneNode : public RefCounted
{
public:
  static const char* NAME;
  static SceneNode* Create();

  SceneNode();
  virtual ~SceneNode() {}
  virtual void Update() {}
  virtual void Draw() {}
  virtual bool Load(File*);
  virtual void CombineTransform();

  // TODO Rename this. Currently makes this node' AABB the union of its child AABBs.
  virtual void UpdateBoundingVol();

  // Transform this node's AABB by matrix m, then all children, recursively
  void RecursivelyTransformAABB(const Matrix& m);

  virtual void BeforeDraw() {}
  virtual void AfterDraw() {}

  void SetParent(SceneNode*);
  SceneNode* GetParent();

  // Overwrite existing transformation
  void SetLocalTransform(const Matrix& mat);

  // Concatenate transform
  void MultLocalTransform(const Matrix& mat);

  const Matrix& GetLocalTransform() const;
  const Matrix& GetCombinedTransform() const;


  bool IsVisible() const;
  bool IsCollidable() const;
  bool IsBlended() const; 
  bool IsCamera() const;

  void SetVisible(bool);
  void SetCollidable(bool);
  void SetBlended(bool);
  void SetIsCamera(bool);

  void AddChild(PSceneNode node);

  AABB* GetAABB();

  void SetColour(const Colour& colour);

protected:
  // Subclasses call this
  bool LoadMatrix(File* f);

  // ? SceneGraph calls this ?
  bool LoadChildren(File* f);

protected:
  friend class SceneGraph; // ???

  Matrix m_local;
  Matrix m_combined;
  SceneNode* m_parent;
  AABB m_aabb;
  uint32 m_flags;
  Colour m_colour;

  typedef std::vector<PSceneNode> Nodes;
  Nodes m_children;
};

typedef Singleton<Factory<SceneNode> > TheSceneNodeFactory;

PSceneNode LoadScene(const std::string& filename);
}

#endif
