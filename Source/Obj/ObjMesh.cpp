#include <assert.h>
#include <ReportError.h>
#include "ObjMesh.h"
#include "File.h"
#include "StringUtils.h"
#include "AmjuGL.h"
#include "CollisionMesh.h"
#include <Matrix.h>

#ifdef _DEBUG
//#define OBJ_DEBUG
#endif

namespace Amju
{
Resource* TextObjLoader(const std::string& resName)
{
  ObjMesh* obj = new ObjMesh;
  if (!obj->Load(resName, 
    false /* text */ ))
  {
    //Assert(0);
    return 0;
  }
  return obj;
}

Resource* BinaryObjLoader(const std::string& resName)
{
  ObjMesh* obj = new ObjMesh;
  if (!obj->Load(resName, 
    true /* binary */ ))
  {
    //Assert(0);
    return 0;
  }
  return obj;
}

void ObjMesh::CalcCollisionMesh(CollisionMesh* pCollMesh)
{
  // Iterate over groups once to count how many faces there are;
  // then iterate again to convert each one to a tri.
  // Don't add tris for groups whose material has 'no collide' flag set.
  pCollMesh->m_tris.clear();

  int numFaces = 0;
  for (Groups::iterator it = m_groups.begin();
    it != m_groups.end();
    ++it)
  {
    Group& g = it->second;
    Material& mat = m_materials[g.m_materialName];
    if (!(mat.m_flags & Material::AMJU_MATERIAL_NO_COLLIDE) || !g.IsCollidable())
    {
      numFaces += g.m_tris.size();
    }
  }
  pCollMesh->m_tris.reserve(numFaces);

  for (Groups::iterator it = m_groups.begin();
    it != m_groups.end();
    ++it)
  {
    Group& g = it->second;
   
    Material& mat = m_materials[g.m_materialName];
    if ((mat.m_flags & Material::AMJU_MATERIAL_NO_COLLIDE) || !g.IsCollidable())
    {
      continue;
    }
    
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
#ifdef OBJ_DEBUG
  std::cout << "Loading binary obj file: " << filename << "\n";
#endif

  File f(File::NO_VERSION);
  if (!f.OpenRead(filename, true /* is binary */))
  {
    return false;
  }

  int n = 0;
  // Points
  f.GetInteger(&n);

#ifdef OBJ_DEBUG
  std::cout << "File supposedly contains " << n << " points\n";
#endif

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

#ifdef OBJ_DEBUG
  std::cout << "Contains " << n << " UVs\n";
#endif

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
#ifdef OBJ_DEBUG
  std::cout << "Contains " << n << " normals\n";
#endif
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
#ifdef OBJ_DEBUG
    std::cout << "Material " << i << ": matName: " << matName << " filename: " << filename << " tex: " << tex << "\n";
#endif

    mat.m_texture = (Texture*)TheResourceManager::Instance()->GetRes(tex);

    if (!mat.m_texture)  
    {
#ifdef OBJ_DEBUG
    std::cout << "FAILED TO LOAD texture " << tex << "!\n";
#endif
      return false;
    }

#ifdef OBJ_DEBUG
    std::cout << "Loaded texture " << tex << " OK!\n";
#endif

    int flags = 0;
    f.GetInteger(&flags);
    mat.m_flags = (uint32)flags;

#ifdef OBJ_DEBUG
    std::cout << "Flags: " << flags << "\n";
#endif
  }

  int numGroups = 0;
  f.GetInteger(&numGroups); 
#ifdef OBJ_DEBUG
  std::cout << "Supposedly " << numGroups << " groups...\n";
#endif

  for (int ig = 0; ig < numGroups; ig++)
  {
    std::string groupName;
    f.GetDataLine(&groupName);
#ifdef OBJ_DEBUG
    std::cout << "Group " << ig << " name: " << groupName << "\n";
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
    else
    {
#ifdef OBJ_DEBUG
      std::cout << "No material for this group.\n";
#endif
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

#ifdef OBJ_DEBUG
  std::cout << "Loading file " << filename << " - not binary\n";
#endif

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
      Vec3f n = ToVec3(strs);
      n.Normalise();
      m_normals.push_back(n);
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
      if (strs.size() == 2)
      {
        currentGroup = strs[1];
        Group& g = m_groups[currentGroup];
        g.m_name = currentGroup;
      }
      else
      {
        f.ReportError("Unexpected format for group");
      }
    }
    else if (strs[0] == "mtllib")
    {
      if (strs.size() != 2)
      {
        f.ReportError("Unexpected format for mtllib");
        continue;
      }

      std::string mtlfilename = strs[1];
      MaterialVec mats;
      if (!LoadMtlFile(mtlfilename, &mats))
      {
        f.ReportError("Failed to load mtl file " + mtlfilename);
        return false;
      }
      //mat.m_filename = mtlfilename;

      for (unsigned int i = 0; i < mats.size(); i++)
      {
        Material& mat = *mats[i];

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
    }
    else if (strs[0] == "usemtl")
    {
      if (strs.size() != 2)
      {
        f.ReportError("Unexpected format for usemtl");
        continue;
      }

      std::string matname = strs[1];
      Group& g = m_groups[currentGroup];
      if (g.m_materialName.empty())
      {
        g.m_materialName = matname;
      }
      else
      {
#ifdef OBJ_DEBUG
        std::cout << "Changing material within the same group - sigh, making new group.\n";
#endif
        // Make name for group
        currentGroup = matname + "_group"; // OK if group name exists
        Group& g = m_groups[currentGroup];
        g.m_name = currentGroup;
        g.m_materialName = matname;
      }
    }
  }

  if (m_points.empty())
  {
#ifdef OBJ_DEBUG
    std::cout << "No point data - failed!\n";
#endif

    return false;
  }

#ifdef OBJ_DEBUG
  std::cout << "Points: " << m_points.size()
    << " UVs: " << m_uvs.size() 
    << " Norms: " << m_normals.size()
    << " Groups: " << m_groups.size()
    << " Mats: " << m_materials.size()
    << "\n";
#endif

  MungeData();
  return true;
}

void ObjMesh::MungeData()
{
#ifdef OBJ_DEBUG
  std::cout << "Optimising .obj data (*cough* because maya is so shit *cough*)...\n";
#endif

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
		
#ifdef WIN32
      it = m_groups.erase(it);
#else
      // See http://stackoverflow.com/questions/52714/stl-vector-vs-map-erase
      m_groups.erase(it);
      ++it;
#endif
    }
    else
    {
      ++it;
    }
  }

  // Remove unused materials
  for (Materials::iterator it = m_materials.begin(); it != m_materials.end(); )
  {
    bool found = false;
    std::string matName = it->first;
    // Look for this name in all groups
    for (Groups::iterator jt = m_groups.begin(); jt != m_groups.end(); ++jt)
    {
      const Group& g = jt->second;
      if (g.m_materialName == matName)
      {
        found = true;
        break;
      }
    }
    if (found)
    {
#ifdef OBJ_DEBUG
      std::cout << "KEEPING material " << matName << "\n";
#endif
      ++it;
    }
    else
    {
#ifdef OBJ_DEBUG
      std::cout << "Removing unused material " << matName << "\n";
#endif
		
#ifdef WIN32
      it = m_materials.erase(it);
#else
      m_materials.erase(it);
      ++it;
#endif
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

void ObjMesh::GetMaterials(MaterialVec* vec)
{
  for (Materials::iterator it = m_materials.begin(); it != m_materials.end(); ++it)
  {
    vec->push_back(new Material(it->second));
  } 
}

Group* ObjMesh::GetGroup(const std::string& groupName)
{
  Groups::iterator it = m_groups.find(groupName);
  if (it == m_groups.end())
  {
    return 0;
  } 
  return &it->second;
}

void ObjMesh::BuildGroup(Group& g)
{
  // Only need to build Tris once - this kind of mesh is not animated!
  Faces& faces = m_facemap[g.m_name];

  unsigned int numfaces = faces.size();

#ifdef OBJ_DEBUG
std::cout << "Group " << g.m_name << " has " << numfaces << " faces.\n";
if (m_uvs.empty())
{
  std::cout << "No UVs in this obj mesh!\n";
}
#endif

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
  if (g.m_tris.empty() || !g.IsVisible())
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

void ObjMesh::Transform(const Matrix& mat)
{
  // Rotate normals but don't translate
  Matrix rotMat = mat;
  rotMat.TranslateKeepRotation(Vec3f(0, 0, 0)); // overwrites any translation

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
        // transform verts; write back to Tri
        p = p * mat; // sigh, TODO
        vn = vn * rotMat;
        vn.Normalise();
        v.m_x = p.x;
        v.m_y = p.y;
        v.m_z = p.z;
        v.m_nx = vn.x;
        v.m_ny = vn.y;
        v.m_nz = vn.z;
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
      of.WriteInteger((int)mat.m_flags);
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
      if (!m.m_filename.empty())
      {
        of.Write("mtllib " + m.m_filename);
      }
      if (!mtlName.empty())
      {
        of.Write("usemtl " + mtlName);
      }

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

    MaterialVec matVec;
    GetMaterials(&matVec);
    if (!SaveMtlFiles(matVec))
    {
      ReportError("Failed to save materials");
      return false;
    }
  }

  return true;
}
}

