//#define AMJU_USE_SDL_IMG

#include <AmjuFirst.h>
#ifdef AMJU_USE_SDL_IMG
#include <SDL_image.h>
#endif

#include "Texture.h"
#include "TextureUtils.h"
#include "Bitmap.h"
#include "ReportError.h"
#include <StringUtils.h>
#include <LoadPng.h>
#include <AmjuFinal.h>

namespace Amju
{
AmjuGL::TextureFilter Texture::s_defaultFilter = AmjuGL::AMJU_TEXTURE_NICE;
AmjuGL::TextureMode Texture::s_defaultWrapMode = AmjuGL::AMJU_TEXTURE_WRAP;

void Texture::SetDefaultFilter(AmjuGL::TextureFilter tf)
{
  s_defaultFilter = tf;
}

void Texture::SetDefaultWrapMode(AmjuGL::TextureMode tm)
{
  s_defaultWrapMode = tm;
}

Texture::Texture()
{
  m_texId = -1;
  m_width = 0;
  m_height = 0;

  m_filter = s_defaultFilter;
  m_wrapMode = s_defaultWrapMode;
  m_textureType = AmjuGL::AMJU_TEXTURE_REGULAR;
}

Texture::~Texture()
{
  AmjuGL::DestroyTextureHandle(&m_texId);
}

void Texture::SetFilter(AmjuGL::TextureFilter tf)
{
  m_filter = tf;
}

void Texture::SetWrapMode(AmjuGL::TextureMode tm)
{
  m_wrapMode = tm;
}

void Texture::SetTextureType(AmjuGL::TextureType tt)
{
  m_textureType = tt;
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

#ifdef AMJU_USE_SDL_IMG

  SDL_Surface* surf = IMG_Load(filename.c_str());
  if (!surf)
  {
    ReportError("Failed to load texture " + filename);
    return false;
  }
  w = surf->w;
  h = surf->h;
  data = (unsigned char*)surf->pixels;
  bpp = surf->format->BytesPerPixel; 

#else

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

#endif

  Create(data, w, h, bpp); 

#ifdef AMJU_USE_SDL_IMAGE
  SDL_FreeSurface(surf);
#else
  delete [] data;
#endif

#ifdef _DEBUG
  m_name = StripPath(filename);
#endif

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
  // Will be false if using console only for debugging
//  Assert(m_texId != (AmjuGL::TextureHandle)-1);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);

  AmjuGL::SetTextureFilter(m_filter); // nicest or nearest
  AmjuGL::SetTextureType(m_textureType); // env map or UV coords
  AmjuGL::SetTextureMode(m_wrapMode); // wrap or clamp

  AmjuGL::UseTexture(m_texId);
}
}
