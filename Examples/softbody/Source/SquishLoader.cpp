#include <iostream>
#include <File.h>
#include <ObjUsefulFunctions.h>
#include <StringUtils.h>
#include "SquishLoader.h"
#include "Squishy.h"

namespace Amju
{
bool ShowInfo()
{
  return true;
}

void MakeSpring(Squishy* sq, int p1, int p2, float k)
{
  int id = sq->CreateSpring(p1, p2);
  Spring* spr = sq->GetSpring(id);
  spr->SetK(k);

  // We have just set the natural length of the spring.
  // Set the min and max lengths based on this.
  // TODO This is a property of the squishy thing
  float natLen = spr->GetNaturalLength();
  // TODO TEMP TEST
  spr->SetMaxLength(natLen * 1.5f);
  spr->SetMinLength(natLen * 0.7f);
}

bool SquishyLoadObj(Squishy* sq, const std::string& filename)
{
  Vecs m_points;
  Vecs m_normals;
  UVs m_uvs;
  FaceMap m_facemap;

  Groups m_groups;

  typedef std::map<std::string, Material> Materials;
  Materials m_materials;
  if (ShowInfo())
  {
    std::cout << "Loading file " << filename << " - text, not binary\n";
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

      if (strs.size() > 4)
      {
        f.ReportError("Non-triangular face, taking first 3 verts only.");
      }

      Face face = ToFace(strs);
      m_facemap[g.m_name].push_back(face);
    }
    else if (strs[0] == "g")
    {
      // Switch current group - create a new group if it doesn't
      //  already exist in the map.
      if (strs.size() == 1)
      {
        f.ReportError("No group name!");
      }
      else if (strs.size() == 2)
      {
        currentGroup = strs[1];
        Group& g = m_groups[currentGroup];
        g.m_name = currentGroup;
      }
      else
      {
        f.ReportError("Info: Unexpected format for group: " + s);
        currentGroup = strs[1];
        for (unsigned int a = 2; a < strs.size(); a++)
        {
          currentGroup += strs[a];
        }
        Group& g = m_groups[currentGroup];
        g.m_name = currentGroup;
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
          if (ShowInfo())
          {
            std::cout << "Discarding material " << mat.m_name << " as no texture\n";
          }
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
        if (ShowInfo())
        {
          std::cout << "Changing material within the same group - sigh, making new group.\n";
        }

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
    if (ShowInfo())
    {
      std::cout << "No point data - failed!\n";
    }
    return false;
  }

  if (ShowInfo())
  {
    std::cout << "Points: " << m_points.size()
      << " UVs: " << m_uvs.size() 
      << " Norms: " << m_normals.size()
      << " Groups: " << m_groups.size()
      << " Mats: " << m_materials.size()
      << "\n";
  }

  // Add a particle for each vertex
  Vec3f centreOfMass;

  for (auto it = m_points.begin(); it != m_points.end(); ++it)
  {
    const Vec3f& v = *it;
    int id = sq->CreateParticle();
    Particle* p = sq->GetParticle(id); // OK this is a bit crap
    p->SetPos(v);
    // Now what about (inv) mass of this particle?
    p->SetInvMass(1.0f); // TODO

    centreOfMass += v;
  }
  
  // Add spring for triangle edges

  // TODO This is SUPER inefficient.
  // Fill up a set with all edges then make the springs -- this way we are testing
  //  for pre-existing springs and getting quadratic complexity.

  typedef std::pair<int, int> Edge;
  typedef std::set<Edge> Edges;
  Edges edges;

  for (auto it = m_facemap.begin(); it != m_facemap.end(); ++it)
  {
    const Faces& faces = it->second;
    for (auto jt = faces.begin(); jt != faces.end(); jt++)
    {
      const Face& f = *jt;

      for (int i = 0; i < 3; i++)
      {
        int a = f.m_pointIndex[i];
        int b = f.m_pointIndex[(i + 1) % 3];
        // Only insert edge if not already there, and check for both ways
        if (edges.count(Edge(a, b)) == 0 && edges.count(Edge(b, a)) == 0)
        {
          edges.insert(Edge(a, b));
        }
      }
/*
      edges.insert(Edge(f.m_pointIndex[0], f.m_pointIndex[1]));
      edges.insert(Edge(f.m_pointIndex[1], f.m_pointIndex[2]));
      edges.insert(Edge(f.m_pointIndex[2], f.m_pointIndex[0]));
*/
    }
  }

  const float K = 0.1f;
  for (auto e = edges.begin(); e != edges.end(); ++e)
  {
    const Edge& edge = *e;
    MakeSpring(sq, edge.first, edge.second, K);
  }

  int numPoints = m_points.size();

  // Idea for constraint to keep shape/volume:
  // Add particle at centre of mass. Add spring from here to each vertex.
  /*
  centreOfMass *= (1.0f / (float)numPoints);

  int centreId = sq->CreateParticle();
  Particle* centreP = sq->GetParticle(centreId); // OK this is a bit crap
  centreP->SetPos(centreOfMass);

  float k = 0.1f; // springiness of the spring to the centre
  for (int i = 0; i < numPoints; i++)
  {
    Assert(i < centreId);
    MakeSpring(sq, centreId, i, k);
  }
  */

  // Here is another idea to keep shape: 
  // Each movable particle has an immovable buddy, connected with a spring.
  // Put the immovable buddy some small distance away from the movable particle,
  //  or the distance will always be approx zero, so no spring action at all.
  
  for (int i = 0; i < numPoints; i++)
  {
    Particle* pOld = sq->GetParticle(i); 

    const Vec3f& v = pOld->GetPos();
    int idNew = sq->CreateParticle();
    Particle* pNew = sq->GetParticle(idNew); 
    pNew->SetPos(v + Vec3f(10, 0, 0));
    pNew->SetInvMass(0); 

    MakeSpring(sq, i, idNew, 1.0f);
  }
  

  return true;
}

}
