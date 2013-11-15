#include <AmjuFirst.h>
#include "Ve1Character.h"
#include <File.h>
#include <DrawAABB.h>
#include "GameMode.h"
#include "Ve1Object.h"
#include "Player.h"
#include "LocalPlayer.h"
#include <AmjuFinal.h>

namespace Amju
{
void Ve1Character::BeforeDraw()
{
}

void Ve1Character::AfterDraw()
{
}

void Ve1Character::Update()
{
  Player* player = dynamic_cast<Player*>(m_obj.GetPtr());
  if (!player)
  {
    return;
  }

  // It's a player: fade out distant players
  // TODO Distance depends if logged in or not. Even logged in players should fade ?

  bool loggedIn = player->IsLoggedIn();

  // For logged out players, only draw if near the local player.
  // We fade in/out with distance. The distance could be configurable.
  // This gives a good frame rate improvement if lots of logged out players.
  if (loggedIn)
  {
    SetBlended(false); // no translucency
  }
  else
  {
    SetAnim(ANIM_IDLE);

    // Show all players in debug mode
#ifndef _DEBUG
    float sqDist = (GetLocalPlayer()->GetPos() - player->GetPos()).SqLen();
    static const float MAX_FADE_DIST = 200.0f; // TODO CONFIG
    static const float MIN_FADE_DIST = 100.0f;
    static const float MAX_FADE_DIST2 = MAX_FADE_DIST * MAX_FADE_DIST;
    static const float MIN_FADE_DIST2 = MIN_FADE_DIST * MIN_FADE_DIST;

    if (sqDist > MAX_FADE_DIST2)
    {
      // Too far away
      SetVisible(false);
      return;
    }
    else if (sqDist > MIN_FADE_DIST2)
    {
      float a = (sqDist - MIN_FADE_DIST2) / (MAX_FADE_DIST2 - MIN_FADE_DIST2);
      //SetColour(Colour(1, 1, 1, 1.0f - a));
      
      // Incoporates code which was in Player::SetLoggedIn()
      SetColour(Colour(0, 0, 0, 0.2f * (1.0f - a)));
      SetBlended(true);
    }
    else
    {
      SetColour(Colour(0, 0, 0, 0.2f));
      SetBlended(true);
    }
#endif

  }
   
   SetVisible(true);
}

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

