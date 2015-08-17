#pragma once

#include <string>
#include <vector>
#include <Cubemap.h>
#include <File.h>
#include <RCPtr.h>
#include <Shader.h>
#include <Texture.h>

namespace Amju
{
struct Material : public RefCounted
{
  std::string m_name;
  std::string m_filename;

  // Textures:
  // In the .mtl file, we allow "map_Kd", "map_Ka" and "map_0" for texture[0].
  // Other textures use map_<n>, where n is [1..MAX_TEXTURES)
  enum { MAX_TEXTURES = 8 };
  std::string m_texfilename[MAX_TEXTURES];
  PTexture m_texture[MAX_TEXTURES];

  // Cubemap: 6 texture names, using "cube_<n>" n=[0..6) in the .mtl file.
  // Same order as in Cubemap.h, 
  // +x (right)
  // -x (left)
  // +z (front)
  // -z (back)
  // +y (top)
  // -y (bottom)
  std::string m_cubefilename[6];
  RCPtr<Cubemap> m_cubemap;

  // Name of shader, passed to AmjuGL::LoadShader
  // In .mtl file, use "shader"
  std::string m_shaderName;
  RCPtr<Shader> m_shader;

  uint32 m_flags;
  // Flags comprised of these values
  enum
  {
    AMJU_MATERIAL_NO_COLLIDE         = 0x01, // 1 to ignore group in collision tests
    AMJU_MATERIAL_SPHERE_MAP         = 0x02, // 1 for sphere mapped
    AMJU_MATERIAL_USE_BLEND_FLAG     = 0x04, // if 1, use next flag
    AMJU_MATERIAL_BLEND_ENABLED      = 0x08, //  ..to enable/disable blending
    AMJU_MATERIAL_USE_LIGHTING_FLAG  = 0x10, // if 1, use next flag
    AMJU_MATERIAL_LIGHTING_ENABLED   = 0x20, //  ..to enable/disable lighting
  };

  Material();
  void UseThisMaterial();
};

typedef std::vector<RCPtr<Material> > MaterialVec;

bool LoadMtlFile(const std::string& mtlfilename, MaterialVec* mats);

// Save a bunch of materials - could be to multiple files. Will overwrite these files.
// This is text .obj mode, because binary mode has materials in the same file as the
//  mesh.
bool SaveMtlFiles(const MaterialVec& mats);


}

