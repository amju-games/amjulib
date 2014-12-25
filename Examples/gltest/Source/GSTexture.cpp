#include <AmjuGL.h>
#include <Vec2.h>
#include <DrawAABB.h>
#include <Texture.h>
#include <ResourceManager.h>
#include <Timer.h>
#include "GSTexture.h"
#include "GSShaderWave.h"

namespace Amju
{
static Texture* m_tex[2] = { 0, 0 };

GSTexture::GSTexture()
{
  m_name = "Texturing";
  m_nextState = TheGSShaderWave::Instance();
  m_maxTime = 7.0f; 
}

void GSTexture::Update()
{
  GSBase::Update();
}

void GSTexture::DrawScene()
{
  DrawHelp();
  AmjuGL::SetClearColour(Colour(1, 0, 0, 1));

  m_tex[0]->UseThisTexture();
    
  static AABB aabb(-1, 1, -1, 1, -1, 1);

  static float f = 0;
  float dt = TheTimer::Instance()->GetDt();
  f += 50.0f * dt;
  AmjuGL::RotateX(f);
  AmjuGL::RotateY(f);

  DrawSolidAABB(aabb);
}

void GSTexture::Draw2d()
{
  GSBase::Draw2d();
}

void GSTexture::OnActive()
{
  GSBase::OnActive();

  static const char* TEX0 = "bw.png";
  static const char* TEX1 = "bw.png";
  //static const char* TEX = "font2d/arial_512_16pt.bmpa";

  m_tex[0] = (Texture*)TheResourceManager::Instance()->GetRes(TEX0);
  m_tex[1] = (Texture*)TheResourceManager::Instance()->GetRes(TEX1);
}

} // namespace
