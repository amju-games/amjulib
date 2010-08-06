#include <map>
#include "LeafLoader.h"
#include "File.h"
#include "ReportError.h"
#include "Vec2.h"
#include "Vec3.h"
#include "ObjMesh.h"
#include "Geometry.h"
#include "StringUtils.h"

namespace Amju
{
ObjMesh* LeafLoad(const std::string& filename)
{
  int normId = 1;
  int vecId = 1;
  int uvId = 1;

  File f;
  if (!f.OpenRead(filename))
  {
    ReportError("Failed to open file");
    return 0;
  }

  int texMethod = 0;
  f.GetInteger(&texMethod);
  // 1: UVs are specified in file 
  // 2: UVs are auto generated -- need to get coefficients
  // 3: Auto generated UVs - env mapped 
  // 4: Null Texture method -- DON'T get texture filename
  // 5: UVs are specified in file, clamped  

  std::string texFilename;
  if (texMethod != 4) // not null tex method
  {
    f.GetDataLine(&texFilename); 
    int alpha = 0;
    f.GetInteger(&alpha);
    if (alpha)
    {
      std::string s;
      f.GetDataLine(&s); // alpha name, discarded
      texFilename += "a"; // add alpha, e.g. "red.bmp" -> "red.bmpa"
    }
  }

  if (texMethod == 2)
  {
    // Auto generated, get coeffs.
    for (int i = 0; i < 8; i++)
    {
      float a = 0;
      f.GetFloat(&a);
    }
  }

  int normalFlag = 0;
  f.GetInteger(&normalFlag);
  int numFaces = 0;
  f.GetInteger(&numFaces);

  typedef std::map<Vec3f, int> VecMap;
  VecMap vecmap;
  Vecs vecs;

  typedef std::map<Vec2f, int> UVMap;
  UVMap uvmap;
  UVs uvs;

  VecMap normalmap;
  Vecs normals;

  Faces faces;

  for (int i = 0; i < numFaces; i++)
  {
    Face face;
    int numVs = 0;
    f.GetInteger(&numVs);
    Assert(numVs == 3);
    Vec3f verts[3];
    for (int j = 0; j < 3; j++)
    {
       Vec3f v;
       f.GetFloat(&v.x);
       f.GetFloat(&v.y);
       f.GetFloat(&v.z);
       verts[j] = v;
       // Add v to map ?
       if (vecmap.find(v) == vecmap.end())
       {
          vecmap[v] = vecId;
          vecs.push_back(v);
          vecId++;
       }
       face.m_pointIndex[j] = vecmap[v];

       if (texMethod == 1) // TODO check this is the right value
       {
         // UVs are listed in file
         Vec2f uv;
         f.GetFloat(&uv.x);
         f.GetFloat(&uv.y);

         if (uvmap.find(uv) == uvmap.end())
         {
           uvmap[uv] = uvId;
           uvs.push_back(uv);
           uvId++;
         }
         face.m_uvIndex[j] = uvmap[uv];
       }
    }
    // Make normal for this face
    Vec3f normal;
    MakeNormal(verts[0], verts[1], verts[2], &normal);
    if (normalmap.find(normal) == normalmap.end())
    {
      normalmap[normal] = normId;
      normals.push_back(normal);
      normId++;
    }
    for (int j = 0; j < 3; j++)
    {
	    face.m_normalIndex[j] = normalmap[normal];
    }

    faces.push_back(face);
  }

  // Smooth normals
  if (normalFlag == 1)
  {
    // Create new list of normals, using the old.
    // Map of vertex indices to normals, which are added to average.
    typedef std::map<int, Vec3f> Smoothed;
    Smoothed smoothed;

    for (unsigned int i = 0; i < faces.size(); i++)
    {
      Face& face = faces[i];
      for (int j = 0; j < 3; j++)
      {
        int vertInd = face.m_pointIndex[j];
        int normInd = face.m_normalIndex[j];
        Vec3f& norm = normals[normInd - 1]; // convert 1-based to 0-based index
        smoothed[vertInd] += norm;
        face.m_normalIndex[j] = vertInd; // replace norm ID
      }
    }
    // Should be exactly as many verts as normals
    Assert(vecs.size() == smoothed.size());

    // Replace normals with the values in smoothed
    normals.clear();
    for (Smoothed::iterator it = smoothed.begin(); it != smoothed.end(); ++it)
    {
      Vec3f n = it->second;
      n.Normalise();
      normals.push_back(n);
    }
  }

  File of(File::NO_VERSION);
  of.OpenWrite(GetFileNoExt(filename) + ".obj");
  of.Write("# j.c. convert leaf file to obj");
  of.Write("# Verts");
  for (unsigned int i = 0; i < vecs.size(); i++)
  {
    of.Write("v " + ToString(vecs[i].x) + " " + ToString(vecs[i].y) + " " + ToString(vecs[i].z));
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
  if (texMethod != 4) // not null tex method
  {
    // Write group name and material
    std::string matName = StripPath(GetFileNoExt(filename));
    of.Write("# Material");
    of.Write("mtllib " + matName + ".mtl");
    of.Write("usemtl " + matName);

    // Write material file - consisting only of texture name
    File mat(File::NO_VERSION);
    mat.OpenWrite(matName + ".mtl");
    mat.Write("# j.c. convert leaf file to obj");
    mat.Write("newmtl " + matName);
    mat.Write("map_Kd " + texFilename);
  }

  of.Write("# Faces");
  for (unsigned int i = 0; i < faces.size(); i++)
  {
    Face& face = faces[i];
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
  return 0;
}
}
