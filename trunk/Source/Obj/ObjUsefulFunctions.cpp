#include "ObjUsefulFunctions.h"
#include "AmjuAssert.h"
#include "StringUtils.h"
#include "File.h"
#include "ResourceManager.h"

namespace Amju
{
bool LoadMtlFile(const std::string& mtlfilename, MaterialVec* mats)
{
  File f(File::NO_VERSION);
  if (!f.OpenRead(mtlfilename))
  {
    Assert(0);
    return false;
  }

  Material* current = 0;

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

    if (strs[0] == "map_Kd" || strs[0] == "map_Ka")
    {
      Assert(strs.size() == 2);

std::cout << "Found texture name " << strs[1] << "\n";

      Assert(current);
      current->m_texfilename = strs[1];
      current->m_texture = (Texture*)TheResourceManager::Instance()->GetRes(current->m_texfilename);
    }
    else if (strs[0] == "newmtl")
    {
      Assert(strs.size() == 2);
      Material* mat = new Material;
      mats->push_back(mat);
      current = mat;

      current->m_name = strs[1];
    }
    else if (strs[0] == "flags")
    {
      Assert(strs.size() == 2);
      current->m_flags = ToInt(strs[1]); 
    }
  }
  return true;
}

Material::Material() : m_flags(0)
{
}

void Material::UseThisMaterial()
{
  if (m_texture)
  {
    m_texture->UseThisTexture();
  }
}

// Convert a vector of four strings to a Vec3. 
// The zeroth string is ignored. Strings 1, 2 & 3 are
//  asssumed to be floats.
Vec3f ToVec3(const Strings& strs)
{
    if (strs.size() != 4)
    {
        Assert(0);
    }
    return Vec3f(ToFloat(strs[1]), ToFloat(strs[2]), ToFloat(strs[3]));
}

// Convert a vector of three strings to a Vec2. 
// The zeroth string is ignored. Strings 1 & 2 are
//  asssumed to be floats.
Vec2f ToVec2(const Strings& strs)
{
    if (strs.size() != 3)
    {
        Assert(0);
    }
    return Vec2f(ToFloat(strs[1]), ToFloat(strs[2]));
}

Face::Face()
{
    m_pointIndex[0] = m_pointIndex[1] = m_pointIndex[2] = 0;
    m_normalIndex[0] = m_normalIndex[1] = m_normalIndex[2] = 0;
    m_uvIndex[0] = m_uvIndex[1] = m_uvIndex[2] = 0;
}

// Convert a vector of strings to a Face.
// The zeroth string is ignored. We only use the first 
//  three vertices.
Face ToFace(const Strings& fstrs)
{
    Face f;
    // strs[1] is "vertex[/UV[/normal]] vertex[/UV[/normal]] ..."
    // Where [ ] means optional. I.e. the data could just be the point index, or
    //  just the point index and UV index.
    // TODO We only handle triangles - handle other size polys ?
    if (fstrs.size() < 4)
    {
        Assert(0);
    }
    for (int i = 0; i < 3 /*strs.size()*/; i++)
    {
        Strings strs = Split(fstrs[i + 1], '/');
        //assert(strs.size() == 3);

        if (strs.size() > 0 && !strs[0].empty())
        {
            f.m_pointIndex[i] = ToInt(strs[0]) - 1;
        }
        if (strs.size() > 1 && !strs[1].empty())
        {
            f.m_uvIndex[i] = ToInt(strs[1]) - 1;
        }
        if (strs.size() > 2 && !strs[2].empty())
        {
            f.m_normalIndex[i] = ToInt(strs[2]) - 1;
        }
    }
    return f;
}
} // namespace
