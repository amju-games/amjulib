
#include "AmjuFirst.h"
#include "Texture.h"
#include "TextureUtils.h"
#include "Bitmap.h"
#include "ReportError.h"
#include <StringUtils.h>
#include <Png/LoadPng.h>
#include "AmjuFinal.h"

namespace Amju
{
Texture::Texture()
{
  m_texId = -1;
  m_width = 0;
  m_height = 0;
}

Texture::~Texture()
{
  AmjuGL::DestroyTextureHandle(&m_texId);
}

AmjuGL::TextureHandle Texture::GetId() const
{
  return m_texId;
}

int Texture::GetWidth() const
{
  return m_width;
}

int Texture::GetHeight() const
{
  return m_height;
}

bool Texture::Load(const std::string& filename)
{
  unsigned int w = 0;
  unsigned int h = 0;
  unsigned char* data = 0;
  unsigned int bpp = 3; // bytes per pixel

  std::string ext = ToLower(GetFileExt(filename));
  if (ext == "bmp")
  {
    data = LoadDIBitmap(filename.c_str(), &w, &h);
  }
  else if (ext == "png")
  {
    data = LoadPng(filename.c_str(), &w, &h, &bpp);
    // TODO I think bmps are upside down, but the rest of the code compensates.. sigh
    FlipBmp(data, w, h, bpp);
  }

  if (!data)
  {
    ReportError("Failed to load texture " + filename);
    return false;
  }

  Create(data, w, h, bpp); 
  delete [] data;
  return true;
}

void Texture::Create(unsigned char* data, int w, int h, int bytesPerPixel)
{
  AmjuGL::TextureDepth td = AmjuGL::AMJU_RGB;
  if (bytesPerPixel == 4)
  {
    td = AmjuGL::AMJU_RGBA;
  }
  AmjuGL::SetTexture(&m_texId, AmjuGL::AMJU_TEXTURE_REGULAR, td, w, h, data);
  m_width = w;
  m_height = h;
}

void Texture::UseThisTexture()
{
  Assert(m_texId != (AmjuGL::TextureHandle)-1);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::UseTexture(m_texId);
}
}
