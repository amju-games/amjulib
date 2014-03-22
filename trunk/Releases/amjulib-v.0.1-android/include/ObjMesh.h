#ifndef OBJ_MESH_H_INCLUDED
#define OBJ_MESH_H_INCLUDED

#include <map>
#include <ResourceManager.h>
#include "ObjUsefulFunctions.h"

namespace Amju
{
class CollisionMesh;
class Matrix;

class ObjMesh : public Resource
{
public:
  bool Load(const std::string& filename, bool binary = false);
  bool Save(const std::string& filename, bool binary = false);

  // Transform all vertices by the given matrix
  void Transform(const Matrix& m);

  void Draw();

  void CalcCollisionMesh(CollisionMesh* pCollMesh);

  // Add groups in om to collection of groups in this mesh
  void Merge(const ObjMesh& om);

  // Get group - so special group names can be used for game-related data.
  // (Do we need to get vec of group names ?)
  Group* GetGroup(const std::string& groupName);

  void GetMaterials(MaterialVec* vec);

  const AABB& GetAABB(); // for entire mesh
  const AABB& GetAABB(const std::string& groupname); // for one group

  static void SetShowInfo(bool);
  static bool ShowInfo();

  // Set flag to fail if we can't load textures specified in materials
  static void SetRequireTextures(bool);
  static bool RequiresTextures();

private:
  bool LoadBinary(const std::string& filename);
  bool SaveBinary(const std::string& filename);

  // Build groups, clean up temp data etc
  void MungeData();

  // TODO Dump this data once loaded ?
  Vecs m_points;
  Vecs m_normals;
  UVs m_uvs;
  FaceMap m_facemap;

  Groups m_groups;

  typedef std::map<std::string, Material> Materials;
  Materials m_materials;

  AABB m_aabb; // union of all group AABBs

private:
  void DrawGroup(Group& g);
  void BuildGroup(Group& g);
};

typedef RCPtr<ObjMesh> PObjMesh;

Resource* TextObjLoader(const std::string& resName);
Resource* BinaryObjLoader(const std::string& resName);

// Used to load an .obj mesh and other associated files (.mtl and textures)
ObjMesh* LoadObjMesh(const std::string& pathFile, bool binary = false);

} // namespace

#endif

