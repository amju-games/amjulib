#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "RCPtr.h"
#include "AmjuTypes.h"
#include "Matrix.h"
#include "Factory.h"
#include "Singleton.h"
#include "AABB.h"

namespace Amju
{
class File;
class SceneNode;
typedef RCPtr<SceneNode> PSceneNode;

// This base class has children - so not really Composite pattern here.
class SceneNode : public RefCounted
{
public:
  static const char* NAME;

  SceneNode();
  virtual ~SceneNode() {}
  virtual void Update() {}
  virtual void Draw() {};
  virtual bool Load(File*);
  virtual void CombineTransform();
  virtual void UpdateBoundingVol();

  // Called from SceneGraph::Draw()
  virtual void DrawChildren();
  virtual void UpdateChildren();

  // Called from DrawChildren()
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

protected:
  // Subclasses call this
  bool LoadMatrix(File* f);

  // ? SceneGraph calls this ?
  bool LoadChildren(File* f);

protected:
  Matrix m_local;
  Matrix m_combined;
  SceneNode* m_parent;
  AABB m_aabb;
  uint32 m_flags;

  typedef std::vector<PSceneNode> Nodes;
  Nodes m_children;
};

typedef Singleton<Factory<SceneNode> > TheSceneNodeFactory;

PSceneNode LoadScene(const std::string& filename);
}

#endif
