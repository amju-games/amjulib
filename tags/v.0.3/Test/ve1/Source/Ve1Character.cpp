#include "Ve1Character.h"
#include <File.h>

namespace Amju
{
bool Ve1Character::Load(File* f)
{
  std::string meshName;
  if (!f->GetDataLine(&meshName))
  {
    f->ReportError("No mesh name for character");
    return false;
  }

  // Load mesh and textures from file
  if (!LoadMd2(meshName))
  {
    return false;
  }

  std::string tex1Name, tex2Name;
  if (!f->GetDataLine(&tex1Name) || !f->GetDataLine(&tex2Name))
  {
    f->ReportError("Failed to get 2 textures for player");
    return false;
  }

  if (!LoadTextures(tex1Name, tex2Name))
  {
    return false;
  }

  return true;
}
}

