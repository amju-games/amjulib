#include "AlphaBmpLoader.h"
#include "ResourceManager.h"
#include "StringUtils.h"
#include "Texture.h"
#include "TextureUtils.h"
#include "Bitmap.h"

namespace Amju
{
Resource* BmpALoader(const std::string& resName)
{
  std::string img = resName.substr(0, resName.size() - 1);
  std::string alpha = GetFileNoExt(img) + "-a.bmp";

  unsigned int w = 0;
  unsigned int h = 0;
  
  unsigned char* rgbdata = LoadDIBitmap(img.c_str(), &w, &h);
  
  if (!rgbdata)
  {
//?    ReportError("Failed to load texture " + img);
    return 0;
  }

  // Add space for alpha
  unsigned char* withAlpha = AddAlpha(rgbdata, w, h);
  Assert(withAlpha);
  delete [] rgbdata;
  rgbdata = 0;

  // Load alpha image
  unsigned int aw = 0;
  unsigned int ah = 0;
  unsigned char* alphadata = LoadDIBitmap(alpha.c_str(), &aw, &ah);
  Assert(alphadata);
  Assert(aw == w);
  Assert(ah == h);

  // Copy alpha image to alpha channel of RGB image
  CopyAlpha(alphadata, withAlpha, w, h);
  delete [] alphadata;
  alphadata = 0;

  Texture* pTex = new Texture;
  pTex->Create(withAlpha, w, h, 4); // 4 bytes per pixel
  delete [] withAlpha;

  return (Resource*)pTex;
}
}
