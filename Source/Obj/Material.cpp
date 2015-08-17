#include "Material.h"
#include <ResourceManager.h>
#include <StringUtils.h>

namespace Amju
{
bool SaveMtlFiles(const MaterialVec& mats)
{
  typedef std::map<std::string, RCPtr<File> > FileMap; // map material filenames to Files
  FileMap fm;

  for (MaterialVec::const_iterator it = mats.begin(); it != mats.end(); ++it)
  {
    const Material* mat = *it;
    if (mat->m_filename.empty())
    {
      continue;
    }

    RCPtr<File>& f = fm[mat->m_filename];
    if (!f)
    {
      f = new File(File::NO_VERSION, File::STD);
      f->OpenWrite(mat->m_filename);
    }

    Assert(!mat->m_name.empty());
    f->Write("newmtl " + mat->m_name);

    for (int i = 0; i < Material::MAX_TEXTURES; i++)
    {
      if (!mat->m_texfilename[i].empty())
      {
        f->Write("map_" + ToString(i) + std::string(" ") + mat->m_texfilename[i]);
      }
    }

    if (!mat->m_shaderName.empty())
    {
      f->Write("shader " + mat->m_shaderName);
    }

    // TODO Cube map texture names

    f->Write("flags " + ToString(mat->m_flags));
    f->Write("Ns 1.0"); // for Maya
  }
  return true;
}

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

    if (strs[0] == "map_Kd" || strs[0] == "map_Ka" || strs[0] == "map_0")
    {
      // .obj/.mtl files can be dodgy
      if (strs.size() != 2)
      {
        f.ReportError("Unexpected format for map_Kd/map_Ka");
        continue;
      }

      Assert(current);
      current->m_texfilename[0] = strs[1];
      current->m_texture[0] = (Texture*)TheResourceManager::Instance()->GetRes(current->m_texfilename[0]);
      if (current->m_texture[0] && (current->m_flags & Material::AMJU_MATERIAL_SPHERE_MAP))
      {
        current->m_texture[0]->SetTextureType(AmjuGL::AMJU_TEXTURE_SPHERE_MAP);
      }
    }
    else if (strs[0].substr(0, 4) == "map_")
    {
      std::string sm = strs[0].substr(4);
      int m = ToInt(sm);
std::cout << "Material: found '" << s << "', that is texture map " << m <<  ", right?\n"; 
      current->m_texfilename[m] = strs[1];
      current->m_texture[m] = (Texture*)TheResourceManager::Instance()->GetRes(current->m_texfilename[m]);
    }
    else if (strs[0] == "shader")
    {
std::cout << "Material: found shader " << strs[1] << "\n";
      current->m_shaderName = strs[1];
      current->m_shader = AmjuGL::LoadShader(current->m_shaderName);
    }
    else if (strs[0] == "newmtl")
    {
      if (strs.size() != 2)
      {
        f.ReportError("Unexpected format for newmtl");
        continue;
      }

      Material* mat = new Material;
      mats->push_back(mat);
      current = mat;
      current->m_filename = mtlfilename;
      current->m_name = strs[1];
    }
    else if (strs[0] == "flags")
    {
      if (strs.size() != 2)
      {
        f.ReportError("Unexpected format for flags");
        continue;
      }

      Assert(strs.size() == 2);
      current->m_flags = ToInt(strs[1]);
      if (current->m_texture && (current->m_flags & Material::AMJU_MATERIAL_SPHERE_MAP))
      {
        current->m_texture[0]->SetTextureType(AmjuGL::AMJU_TEXTURE_SPHERE_MAP);
      }
    }
  }
  return true;
}

Material::Material() : m_flags(0)
{
}

void Material::UseThisMaterial()
{
  if (m_shader)
  {
    AmjuGL::UseShader(m_shader);
  }

  for (int i = 0; i < MAX_TEXTURES; i++)
  {
    if (m_texture[i])
    {
      // TODO Set unit ID
      m_texture[i]->UseThisTexture(i);
    }
  }

  if (m_cubemap)
  {
    // TODO
  }

  if ((m_flags & AMJU_MATERIAL_USE_BLEND_FLAG) != 0)
  {
    if ((m_flags & AMJU_MATERIAL_BLEND_ENABLED) != 0)
    {
      AmjuGL::Enable(AmjuGL::AMJU_BLEND);
    }
    else
    {
      AmjuGL::Disable(AmjuGL::AMJU_BLEND);
    }
  }

  if ((m_flags & AMJU_MATERIAL_USE_LIGHTING_FLAG) != 0)
  {
    if ((m_flags & AMJU_MATERIAL_LIGHTING_ENABLED) != 0)
    {
      AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
    }
    else
    {
      AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
    }
  }
}

}

