
#include "AmjuFirst.h"
#include "Texture.h"
#include "TextureUtils.h"
#include "Bitmap.h"
#include "ReportError.h"
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
  // TODO Support png
  unsigned int w = 0;
  unsigned int h = 0;
  
  unsigned char* data = LoadDIBitmap(filename.c_str(), &w, &h);
  
  if (!data)
  {
    ReportError("Failed to load texture " + filename);
    return false;
  }

  Create(data, w, h, 3); // 3 bytes per pixel
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
  AmjuGL::UseTexture(m_texId);
}
}
