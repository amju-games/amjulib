#include <LoadPng.h>
#include <StringUtils.h>
#include "Internal/OpenGL.h"
#include "AmjuGL-OpenGL-Base.h"
#include "CubemapOpenGL.h"

namespace Amju
{
static GLint cubeMapSides[] =
{
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
};

bool CubemapOpenGL::Init() 
{
  GLuint texture;
  GL_CHECK(glGenTextures(1, &texture));

  GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, texture));
  GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT));
  GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT));
  GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

  // Upload data for each face
  for (int i = 0; i < 6; i++)
  {
    unsigned int w = 0;
    unsigned int h = 0;
    unsigned char* data = 0;
    unsigned int bpp = 3; // bytes per pixel

    const std::string& filename = m_textureNames[i];
    Assert(GetFileExt(filename) == "png");
    data = LoadPng(filename.c_str(), &w, &h, &bpp);
 
    if (bpp == 3)
    {
      GL_CHECK(glTexImage2D(cubeMapSides[i], 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    }
    else if (bpp == 4)
    {
      GL_CHECK(glTexImage2D(cubeMapSides[i], 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    }
    // TODO Now make mipmaps here?
  }

  return true;
}

void CubemapOpenGL::Draw() 
{
  GL_CHECK(glActiveTexture(m_textureUnitId));
  GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texId));
}

}

