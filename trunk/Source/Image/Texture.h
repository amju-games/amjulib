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
  void UseThisTexture();
  int GetWidth() const;
  int GetHeight() const;
  
private:
  AmjuGL::TextureHandle m_texId;
  int m_width;
  int m_height;
};

typedef RCPtr<Texture> PTexture;
}

#endif
