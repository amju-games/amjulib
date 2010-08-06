#include <assert.h>
#include "ObjMesh.h"
#include "File.h"
#include "StringUtils.h"
#include "AmjuGL.h"
#include "CollisionMesh.h"

//#define OBJ_DEBUG

namespace Amju
{
Resource* ObjLoader(const std::string& resName)
{
  ObjMesh* obj = new ObjMesh;
  if (!obj->Load(resName, 
#ifdef NO_COMPILED_ASSETS
    false /* text */ ))
#else
    true /* binary */ ))
#endif // NO_COMPILED_ASSETS
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

bool ObjMesh::SaveBinary(const std::string& filename)
{
  return true;
}

bool ObjMesh::LoadBinary(const std::string& filename)
{
  File f(File::NO_VERSION);
  if (!f.OpenRead(filename, true /* is binary */))
  {
    return false;
  }

  int n = 0;
  // Points
  f.GetInteger(&n);
  m_points.reserve(n);
  for (int i = 0; i < n; i++)
  {
    Vec3f v;
    f.GetFloat(&v.x);
    f.GetFloat(&v.y);
    f.GetFloat(&v.z);
    m_points.push_back(v);
  }
#ifdef OBJ_DEBUG
  std::cout << "Loaded " << n << " points\n";
#endif

  // UVs
  f.GetInteger(&n);
  m_uvs.reserve(n);
  for (int i = 0; i < n; i++)
  {
    Vec2f v;
    f.GetFloat(&v.x);
    f.GetFloat(&v.y);
    m_uvs.push_back(v);
  }
#ifdef OBJ_DEBUG
  std::cout << "Loaded " << n << " UVs\n";
#endif

  // Normals
  f.GetInteger(&n);
  m_normals.reserve(n);
  for (int i = 0; i < n; i++)
  {
    Vec3f v;
    f.GetFloat(&v.x);
    f.GetFloat(&v.y);
    f.GetFloat(&v.z);
    m_normals.push_back(v);
  }
#ifdef OBJ_DEBUG
  std::cout << "Loaded " << n << " normals\n";
#endif

  // Load materials
  int numMats = 0;
  f.GetInteger(&numMats);
#ifdef OBJ_DEBUG
  std::cout << "Loading " << numMats << " materials..\n";
#endif

  for (int i = 0; i < numMats; i++)
  {
    std::string matName;
    f.GetDataLine(&matName);
    Material& mat = m_materials[matName];
    mat.m_name = matName;

    std::string filename;
    f.GetDataLine(&filename);
    mat.m_filename = filename;

    std::string tex;
    f.GetDataLine(&tex);
    mat.m_texfilename = tex;

    mat.m_texture = (Texture*)TheResourceManager::Instance()->GetRes(tex);
#ifdef OBJ_DEBUG
    std::cout << "  Loaded texture " << tex << "\n";
#endif
  }

  int numGroups = 0;
  f.GetInteger(&numGroups); 
  for (int ig = 0; ig < numGroups; ig++)
  {
    std::string groupName;
    f.GetDataLine(&groupName);
#ifdef OBJ_DEBUG
    std::cout << "Group: " << groupName << "\n";
#endif

    Group& g = m_groups[groupName];
    g.m_name = groupName;

    // Get flag - 1 if we have material 
    int matFlag = 0;
    f.GetInteger(&matFlag);
    if (matFlag)
    {
      std::string matFilename;
      f.GetDataLine(&matFilename);
#ifdef OBJ_DEBUG
      std::cout << "Material file: " << matFilename << "\n";
#endif
      // TODO Do we need this ?

      std::string matName;
      f.GetDataLine(&matName);
#ifdef OBJ_DEBUG
      std::cout << "Material name: " << matName << "\n";
#endif
      g.m_materialName = matName;
    }

    // Load face info
    int numFaces = 0;
    f.GetInteger(&numFaces);

    m_facemap[g.m_name].reserve(numFaces);
    for (int i = 0; i < numFaces; i++)
    {
      Face face;
      for (int j = 0; j < 3; j++)
      {
        f.GetInteger(&face.m_pointIndex[j]);
        f.GetInteger(&face.m_uvIndex[j]);
        f.GetInteger(&face.m_normalIndex[j]);
      }
      m_facemap[g.m_name].push_back(face);
    }
#ifdef OBJ_DEBUG
    std::cout << "Loaded " << numFaces << " faces\n";
#endif
  }

  MungeData();
  return true;
}

bool ObjMesh::Load(const std::string& filename, bool binary)
{
  if (binary)
  {
    return LoadBinary(filename);
  }

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
  // TODO Remove any empty groups
  Group& g = m_groups[currentGroup];
  g.m_name = currentGroup;

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
      Assert(!g.m_name.empty());
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
      // This may be wrong!
      std::string mtlfilename = strs[1];
      Material mat;
      mat.Load(mtlfilename);
      mat.m_filename = mtlfilename;
      Assert(!mat.m_name.empty());
      // Only add if texture specified 
      if (!mat.m_texfilename.empty())
      {
        m_materials[mat.m_name] = mat;
      }
      else
      {
#ifdef OBJ_DEBUG
        std::cout << "Discarding material " << mat.m_name << " as no texture\n";
#endif
      }
    }
    else if (strs[0] == "usemtl")
    {
      std::string matname = strs[1];
      Group& g = m_groups[currentGroup];
      g.m_materialName = matname;
    }
  }

  MungeData();
  return true;
}

void ObjMesh::MungeData()
{
  for (Groups::iterator it = m_groups.begin();
    it != m_groups.end();
    /* increment in body */)
  {
    Group& g = it->second;
    BuildGroup(g);

    // Erase empty groups
    if (g.m_tris.empty())
    {
#ifdef OBJ_DEBUG
      std::cout << "Removing empty group " << g.m_name << "\n";
#endif
		
      // See http://stackoverflow.com/questions/52714/stl-vector-vs-map-erase
      m_groups.erase(it);
      ++it;
    }
    else
    {
      ++it;
    }
  }

  // Remove data no longer needed (only used to create group data)
  m_points.clear();
  m_normals.clear();
  m_uvs.clear();
  m_facemap.clear();

#ifdef OBJ_DEBUG
  std::cout << "Finished\n";
#endif
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
        unsigned int n = f.m_normalIndex[j];
        Assert(n < m_normals.size());
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
        unsigned int uv = f.m_uvIndex[j];
        Assert(uv < m_uvs.size());
        const Vec2f& vUV = m_uvs[uv];

        t.m_verts[j].m_u = vUV.x;
        t.m_verts[j].m_v = vUV.y;
      }

      Assert(!m_points.empty());

      unsigned int p = f.m_pointIndex[j];
      Assert(p < m_points.size());
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

bool ObjMesh::Save(const std::string& filename, bool binary)
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
    Assert(!it->first.empty()); // no group name ?!

    Group& g = it->second;
    Assert(!g.m_name.empty());
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

  if (binary)
  {
    File of(File::NO_VERSION);
    of.OpenWrite(filename, 0, true /* is binary */);

    // Save points
#ifdef OBJ_DEBUG
    std::cout << "Saving " << points.size() << " points\n";
#endif
    of.WriteInteger(points.size());
    for (unsigned int i = 0; i < points.size(); i++)
    {
      const Vec3f& v = points[i];
      of.WriteFloat(v.x);
      of.WriteFloat(v.y);
      of.WriteFloat(v.z);
    }

    // Save UVs
#ifdef OBJ_DEBUG
    std::cout << "Saving " << uvs.size() << " UVs\n";
#endif
    of.WriteInteger(uvs.size());
    for (unsigned int i = 0; i < uvs.size(); i++)
    {
      const Vec2f& v = uvs[i];
      of.WriteFloat(v.x);
      of.WriteFloat(v.y);
    }

    // Save normals
#ifdef OBJ_DEBUG
    std::cout << "Saving " << normals.size() << " normals\n";
#endif
    of.WriteInteger(normals.size());
    for (unsigned int i = 0; i < normals.size(); i++)
    {
      const Vec3f& v = normals[i];
      of.WriteFloat(v.x);
      of.WriteFloat(v.y);
      of.WriteFloat(v.z);
    }

    // Save materials
#ifdef OBJ_DEBUG
    std::cout << "Saving " << m_materials.size() << " materials\n";
#endif
    of.WriteInteger(m_materials.size());
    for (Materials::iterator it = m_materials.begin(); it != m_materials.end(); ++it)
    {
      Material& mat = it->second;
      of.Write(mat.m_name);
      of.Write(mat.m_filename);
      of.Write(mat.m_texfilename);
    }

    // Save groups
#ifdef OBJ_DEBUG
    std::cout << "Saving " << groupMap.size() << " groups\n";
#endif
    of.WriteInteger(groupMap.size());
    for (SaveGroupMap::iterator it = groupMap.begin();
      it != groupMap.end();
      ++it)
    {
      SaveGroup& sg = it->second;
      std::string groupName = it->first;
      Group& g = m_groups[groupName];
      std::string mtlName = g.m_materialName;
      Material& m = m_materials[mtlName];

      // TODO Use ID for groups, just need to be unique
      // TODO Write all materials to this file. Use IDs instead of names.

      // Save group info
      // NB If we use names, we can go back from binary to text
      of.Write(groupName);

      if (!mtlName.empty() && !m.m_filename.empty())
      {
        of.WriteInteger(1); // means there is material for this group
        of.Write(m.m_filename);
        of.Write(mtlName); 
      }
      else
      {
        of.WriteInteger(0); // means no material for this group
      }

      // Save face info
      of.WriteInteger(sg.m_faces.size());
#ifdef OBJ_DEBUG
      std::cout << "Saving " << sg.m_faces.size() << " faces\n";
#endif
      for (unsigned int i = 0; i < sg.m_faces.size(); i++)
      {
        Face& face = sg.m_faces[i];
        for (int j = 0; j < 3; j++)
        {
          Assert(face.m_pointIndex[j] > 0);
          Assert(face.m_uvIndex[j] > 0);
          Assert(face.m_normalIndex[j] > 0);

          // Indices are one-based, save as zero-based in binary format
          of.WriteInteger(face.m_pointIndex[j] - 1);
          of.WriteInteger(face.m_uvIndex[j] - 1);
          of.WriteInteger(face.m_normalIndex[j] - 1);
        }
      }
    }
  }
  else
  {
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
      std::string groupName = it->first;
      Group& g = m_groups[groupName];
      std::string mtlName = g.m_materialName;
      Material& m = m_materials[mtlName];

      of.Write("g " + groupName);

      // TODO This works up to a point, but only if materials are one per file.
      of.Write("mtllib " + m.m_filename);
      of.Write("usemtl " + mtlName);

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
  }

  return true;
}
}

