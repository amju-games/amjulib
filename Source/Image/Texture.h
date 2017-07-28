#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include "RCPtr.h"
#include "AmjuGL.h"

namespace Amju
{
class Texture : public RefCounted
{
public:
  Texture();
  ~Texture();
  bool Load(const std::string& filename);
  void Create(unsigned char* data, int w, int h, int bytesPerPixel);
  void UseThisTexture(int textureUnitId = 0);
  int GetWidth() const;
  int GetHeight() const;
  int GetBytesPerPixel() const;
  
  AmjuGL::TextureHandle GetId() const;

  void SetFilter(AmjuGL::TextureFilter);
  void SetWrapMode(AmjuGL::TextureMode);
  void SetTextureType(AmjuGL::TextureType);

  static void SetDefaultFilter(AmjuGL::TextureFilter);
  static void SetDefaultWrapMode(AmjuGL::TextureMode);

private:
  AmjuGL::TextureHandle m_texId = -1;
  int m_width = 0;
  int m_height = 0;
  int m_bytesPerPixel = 0;
  AmjuGL::TextureFilter m_filter;
  AmjuGL::TextureMode m_wrapMode;
  AmjuGL::TextureType m_textureType;

  static AmjuGL::TextureFilter s_defaultFilter;
  static AmjuGL::TextureMode s_defaultWrapMode;
  // Default type is always UV

#ifdef _DEBUG
  std::string m_name;
#endif
};

typedef RCPtr<Texture> PTexture;
}

#endif
