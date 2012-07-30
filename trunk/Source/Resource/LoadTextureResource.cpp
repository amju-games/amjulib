#include "LoadTextureResource.h"
#include "Texture.h"
#include "File.h"
#include "ResourceManager.h"

namespace Amju
{
Texture* LoadTextureResource(File* f)
{
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected texture name");
    return 0;
  }
  Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes(s);
  Assert(tex);
  return tex;
}
}
