#ifndef OBJ_MESH_H_INCLUDED
#define OBJ_MESH_H_INCLUDED

#include <map>
#include "ObjUsefulFunctions.h"
#include "ResourceManager.h"
#include "Matrix.h"

namespace Amju
{
class CollisionMesh;

class ObjMesh : public Resource
{
public:
  bool Load(const std::string& filename);
  bool Save(const std::string& filename);

  // Transform all vertices by the given matrix
  void Transform(const Matrix& m);

  void Draw();

  void CalcCollisionMesh(CollisionMesh* pCollMesh);

  // Add groups in om to collection of groups in this mesh
  void Merge(const ObjMesh& om);



private:
  // TODO Dump this data once loaded ?
  Vecs m_points;
  Vecs m_normals;
  UVs m_uvs;
  FaceMap m_facemap;

  Groups m_groups;

  typedef std::map<std::string, Material> Materials;
  Materials m_materials;

private:
  void DrawGroup(Group& g);
  void BuildGroup(Group& g);
};

typedef RCPtr<ObjMesh> PObjMesh;

Resource* ObjLoader(const std::string& resName);

} // namespace

#endif

