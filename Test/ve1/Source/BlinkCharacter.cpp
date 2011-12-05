#include "BlinkCharacter.h"
#include "Timer.h"
#include "ResourceManager.h"

namespace Amju
{
BlinkCharacter::BlinkCharacter()
{
  m_blinkTime = 0;
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
