#include <assert.h>
#include "ObjMesh.h"
#include "File.h"
#include "StringUtils.h"
#include "AmjuGL.h"
#include "CollisionMesh.h"

namespace Amju
{
Resource* ObjLoader(const std::string& resName)
{
  ObjMesh* obj = new ObjMesh;
  if (!obj->Load(resName))
  {
    Assert(0);
    return 0;
  }
  return obj;
}

void ObjMesh::CalcCollisionMesh(CollisionMesh* pCollMesh)
{
  // Iterate over groups once to count how many faces there are;
  // then iterate again to convert each one to a tri.
  int numFaces = 0;
  for (Groups::iterator it = m_groups.begin();
    it != m_groups.end();
    ++it)
  {
    Group& g = it->second;
    numFaces += g.m_tris.size();
  }
  pCollMesh->m_tris.reserve(numFaces);

  for (Groups::iterator it = m_groups.begin();
    it != m_groups.end();
    ++it)
  {
    Group& g = it->second;
    unsigned int numTris = g.m_tris.size();
    for (unsigned int i = 0; i < numTris; i++)
    {
      AmjuGL::Tri& tri = g.m_tris[i];
      Tri t(
        Vec3f(tri.m_verts[0].m_x, tri.m_verts[0].m_y, tri.m_verts[0].m_z),
        Vec3f(tri.m_verts[1].m_x, tri.m_verts[1].m_y, tri.m_verts[1].m_z),
        Vec3f(tri.m_verts[2].m_x, tri.m_verts[2].m_y, tri.m_verts[2].m_z));

      pCollMesh->m_tris.push_back(t);
    }
  }
}

bool ObjMesh::Load(const std::string& filename)
{
  File f(File::NO_VERSION);
  if (!f.OpenRead(filename))
  {
    return false;
  }

  // Start off with a default group name - using the filename makes
  //  it unique, so we can merge obj files.
  std::string currentGroup = filename;

  // Set name for default group..?
  // Bad idea, as we can end up with an empty group
  //  Group& g = m_groups[currentGroup];
  //  g.m_name = currentGroup;

  while (true)
  {
    std::string s;
    if (!f.GetDataLine(&s))
    {
      break;
    }
    Trim(&s);
    if (s.empty())
    {
      continue;
    }

    Strings strs = Split(s, ' ');
    Assert(!strs.empty());

    if (strs[0] == "v")
    {
      m_points.push_back(ToVec3(strs));
    }
    else if (strs[0] == "vn")
    {
      m_normals.push_back(ToVec3(strs));
    }
    else if (strs[0] == "vt")
    {
      m_uvs.push_back(ToVec2(strs));
    }
    else if (strs[0] == "f")
    {
      Group& g = m_groups[currentGroup];
      Face face = ToFace(strs);
      m_facemap[g.m_name].push_back(face);
    }
    else if (strs[0] == "g")
    {
      // Switch current group - create a new group if it doesn't
      //  already exist in the map.
      currentGroup = strs[1];
      Group& g = m_groups[currentGroup];
      g.m_name = currentGroup;
    }
    else if (strs[0] == "mtllib")
    {
      // TODO One material is specified in one .mtl file, right ?
      std::string mtlfilename = strs[1];
      Material mat;
      mat.Load(mtlfilename);
      Assert(!mat.m_name.empty());
      m_materials[mat.m_name] = mat;
    }
    else if (strs[0] == "usemtl")
    {
      std::string matname = strs[1];
      Group& g = m_groups[currentGroup];
      g.m_materialName = matname;
    }
  }

  for (Groups::iterator it = m_groups.begin();
    it != m_groups.end();
    ++it)
  {
    Group& g = it->second;
    BuildGroup(g);
  }

  return true;
}

void ObjMesh::Draw()
{
  for (Groups::iterator it = m_groups.begin();
       it != m_groups.end();
       ++it)
  {
      Group& g = it->second;
      DrawGroup(g);
  }
}

void ObjMesh::BuildGroup(Group& g)
{
  // Only need to build Tris once - this kind of mesh is not animated!
  Faces& faces = m_facemap[g.m_name];

  unsigned int numfaces = faces.size();
  g.m_tris.reserve(numfaces);
  for (unsigned int i = 0; i < numfaces; i++)
  {
    const Face& f = faces[i];
    AmjuGL::Tri t;
    for (int j = 0; j < 3; j++)
    {
      if (!m_normals.empty())
      {
        int n = f.m_normalIndex[j];
        const Vec3f& vN = m_normals[n];

        t.m_verts[j].m_nx = vN.x;
        t.m_verts[j].m_ny = vN.y;
        t.m_verts[j].m_nz = vN.z;
      }

      if (m_uvs.empty())
      {
        t.m_verts[j].m_u = 0;
        t.m_verts[j].m_v = 0;
      }
      else
      {
        int uv = f.m_uvIndex[j];
        const Vec2f& vUV = m_uvs[uv];

        t.m_verts[j].m_u = vUV.x;
        t.m_verts[j].m_v = vUV.y;
      }

      Assert(!m_points.empty());

      int p = f.m_pointIndex[j];
      const Vec3f vP = m_points[p];

      t.m_verts[j].m_x = vP.x;
      t.m_verts[j].m_y = vP.y;
      t.m_verts[j].m_z = vP.z;
    }
    g.m_tris.push_back(t);
  }
}

void ObjMesh::DrawGroup(Group& g)
{
  if (g.m_tris.empty())
  {
    return;
  }

  // TODO Hash, not string
  Materials::iterator it = m_materials.find(g.m_materialName);
  if (it != m_materials.end())
  {
    Material& mat = it->second;
    mat.UseThisMaterial();
  }

  AmjuGL::DrawTriList(g.m_tris);
}

void ObjMesh::Merge(const ObjMesh& om)
{
  m_materials.insert(om.m_materials.begin(), om.m_materials.end());
  m_groups.insert(om.m_groups.begin(), om.m_groups.end());
}

void ObjMesh::Transform(const Matrix& m)
{
  for (Groups::iterator it = m_groups.begin();
    it != m_groups.end();
    ++it)
  {
    Group& g = it->second;

    for (unsigned int i = 0; i < g.m_tris.size(); i++)
    {
      AmjuGL::Tri& tri = g.m_tris[i];
      for (int j = 0; j < 3; j++)
      {
        AmjuGL::Vert& v = tri.m_verts[j];

        Vec3f p(v.m_x, v.m_y, v.m_z);
        Vec3f vn(v.m_nx, v.m_ny, v.m_nz);
        // TODO transform verts; write back to Tri
      }
    }
  }
}

bool ObjMesh::Save(const std::string& filename)
{
  typedef std::map<Vec3f, int> Vec3Map;
  Vec3Map pointMap;
  Vecs points;
  Vec3Map normalMap;
  Vecs normals;
  typedef std::map<Vec2f, int> Vec2Map;
  Vec2Map uvMap;
  UVs uvs;

  typedef std::map<std::string, SaveGroup> SaveGroupMap;
  SaveGroupMap groupMap;

  for (Groups::iterator it = m_groups.begin();
    it != m_groups.end();
    ++it)
  {
    Group& g = it->second;
    SaveGroup& sg = groupMap[g.m_name];

    for (unsigned int i = 0; i < g.m_tris.size(); i++)
    {
      AmjuGL::Tri& tri = g.m_tris[i];
      Face face;

      for (int j = 0; j < 3; j++)
      {
        AmjuGL::Vert& v = tri.m_verts[j];

        Vec3f p(v.m_x, v.m_y, v.m_z);
        if (pointMap.find(p) == pointMap.end())
        {
          pointMap[p] = points.size() + 1; // 1-based
          points.push_back(p);
        }
        Vec3f vn(v.m_nx, v.m_ny, v.m_nz);
        if (normalMap.find(vn) == normalMap.end())
        {
          normalMap[vn] = normals.size() + 1;
          normals.push_back(vn);
        }
        Vec2f vt(v.m_u, v.m_v);
        if (uvMap.find(vt) == uvMap.end())
        {
          uvMap[vt] = uvs.size() + 1;
          uvs.push_back(vt);
        }

        face.m_pointIndex[j] = pointMap[p];
        face.m_normalIndex[j] = normalMap[vn];
        face.m_uvIndex[j] = uvMap[vt];
      }
      sg.m_faces.push_back(face);
    }
  }

  File of(File::NO_VERSION);
  of.OpenWrite(filename);
  of.Write("# j.c. saved from ObjMesh class");

  // Write all points
  of.Write("# points");
  for (unsigned int i = 0; i < points.size(); i++)
  {
    of.Write("v " + ToString(points[i].x) + " " + ToString(points[i].y) + " " + ToString(points[i].z));
  }
  of.Write("# UVs");
  for (unsigned int i = 0; i < uvs.size(); i++)
  {
    of.Write("vt " + ToString(uvs[i].x) + " " + ToString(uvs[i].y));
  }
  of.Write("# Normals");
  for (unsigned int i = 0; i < normals.size(); i++)
  {
    of.Write("vn " + ToString(normals[i].x) + " " + ToString(normals[i].y) + " " + ToString(normals[i].z));
  }
  for (SaveGroupMap::iterator it = groupMap.begin();
    it != groupMap.end();
    ++it)
  {
    SaveGroup& sg = it->second;
    of.Write("g " + it->first);

    // TODO Use Material for this group!
    of.Write("mtllib default.mtl");
    of.Write("usemtl default");

    of.Write("# Faces");
    for (unsigned int i = 0; i < sg.m_faces.size(); i++)
    {
      Face& face = sg.m_faces[i];
      std::string s = "f ";
      for (int j = 0; j < 3; j++)
      {
        s += ToString(face.m_pointIndex[j]) 
          + "/" 
          + (face.m_uvIndex[j] ? ToString(face.m_uvIndex[j]) : "")
          + "/"
          + (face.m_normalIndex[j] ? ToString(face.m_normalIndex[j]) : "")
          + " ";
      }
      of.Write(s);
    }
  }

  return true;
}
}

