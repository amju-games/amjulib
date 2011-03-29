
#ifndef AMJU_INDEXED_MESH_H_INCLUDED
#define AMJU_INDEXED_MESH_H_INCLUDED

#include "SharedPtr.h"
#include "Skin.h"
#include "AmjuGL.h"
//#include "TextureMethod.h"

namespace Amju
{
class File;

class IndexedMesh : public Shareable
{
public:
  bool Load(File*);
  bool Save(File*);

  // Draw, passing in transformed bone matrices. We use them and the Skin (bone
  //  weights) to transform each vertex.
  // If no bones are passed in, or there is no skin, the mesh is not transformed.
  void Draw(Bone* arrayOfBones = 0);  

  void SetSkin(PSkin);

private:
  // List of bones and weights
  PSkin m_pSkin;

  // TODO Don't need UVs in here, and possibly we won't have normals either
  //  -- so use Vec3 instead
  AmjuGL::Verts m_verts;
  AmjuGL::UVs m_uvs;
  AmjuGL::IndexedTriList m_tris;

  // Method of texturing: regular, auto-generated or env-mapped etc.
  // TODO DEPENDENCY
  //SharedPtr<TextureMethod> m_pTextureMethod;
};

typedef SharedPtr<IndexedMesh> PMesh;
}

#endif
