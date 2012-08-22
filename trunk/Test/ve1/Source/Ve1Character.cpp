#include "Ve1Character.h"
#include <File.h>
#include <DrawAABB.h>
#include "GameMode.h"
#include "Ve1Object.h"

namespace Amju
{
void Ve1Character::Draw()
{
  // Subclasses override. Use before/after to show selected - TODO

/*
  if (GetGameMode() == AMJU_MODE_EDIT)
  {
    PushColour();
    if (m_obj->IsSelected())
    {
      MultColour(Colour(1, 0, 0, 1));
    }
    else
    {
      MultColour(Colour(0, 0, 1, 1)); // So visible, grey is not very noticeable
    }

    Assert(GetAABB());
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
    DrawAABB(*(GetAABB()));
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);

    PopColour();
  }

  BlinkCharacter::Draw();
*/
}

bool Ve1Character::Load(File* f)
{
/*
  // This is a character name which avatar manager uses to create scene node object and load data
  // ????
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
*/

  return true;
}
}

