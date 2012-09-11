#include "BlinkCharacter.h"
#include "Timer.h"
#include "ResourceManager.h"

namespace Amju
{
BlinkCharacter::BlinkCharacter()
{
  m_blinkTime = 0;
}

void BlinkCharacter::SetFromCharacterName(const std::string& chName)
{
  Animated::SetFromCharacterName(chName);
  // Use a strict naming convention for the two texture files; otherwise read the names from another
  //  well-named text file.

  // TODO static subdir string
  std::string tex1 = "characters/" + chName + "/" + chName + ".png";
  std::string tex2 = "characters/" + chName + "/" + chName + "a.png";

//std::cout << "Loading textures for blink char: " << tex1 << "  and  " << tex2 << "\n";

  LoadTextures(tex1, tex2);
}

void BlinkCharacter::SetTex(Texture* t1, Texture* t2)
{
  Assert(t1);
  Assert(t2);

  m_pTex[0] = t1;
  m_pTex[1] = t2;
}

bool BlinkCharacter::LoadTextures(const std::string& tex1, const std::string& tex2)
{
  m_pTex[0] = (Texture*)TheResourceManager::Instance()->GetRes(tex1);
  Assert(m_pTex[0]);

  m_pTex[1] = (Texture*)TheResourceManager::Instance()->GetRes(tex2);
  Assert(m_pTex[1]);

  return true;
}

void BlinkCharacter::Update()
{
  Animated::Update();

  m_blinkTime += TheTimer::Instance()->GetDt();
  if (m_blinkTime > 3.2f)
  {
    m_blinkTime = 0;
  }
}

void BlinkCharacter::Draw()
{
  if (!m_pTex[0] || !m_pTex[1])
  {
std::cout << "Warning: BlinkChar: can't draw, textures not set yet.\n";
    return;
  }

  int blink = 0;

  // TODO CONFIG ? Or random
  if (m_blinkTime > 3.0f && m_blinkTime <= 3.2f)
  {
    blink = 1;
  }

  m_pTex[blink]->UseThisTexture();

  Animated::Draw();
}
}
