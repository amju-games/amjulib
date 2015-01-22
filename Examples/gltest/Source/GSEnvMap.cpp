#include <AmjuGL.h>
#include <Teapot.h>
#include <ResourceManager.h>
#include <Texture.h>
#include <Timer.h>
#include "GSEnvMap.h"
#include "GSFinish.h"

namespace Amju
{
GSEnvMap::GSEnvMap()
{
  m_name = "Sphere mapping";
  m_nextState = TheGSFinish::Instance();
  m_maxTime = 100.0f;
}

void GSEnvMap::Update()
{
  GSBase::Update();
}

void GSEnvMap::DrawScene()
{
  AmjuGL::SetClearColour(Colour(0.2f, 0.2f, 0.2f, 1));
  GSBase::DrawHelp();

  static Teapot tp;

  static Texture* tex = 
    (Texture*)TheResourceManager::Instance()->GetRes("spheremap_bar.png");

  tex->SetTextureType(AmjuGL::AMJU_TEXTURE_SPHERE_MAP);
  tex->UseThisTexture();

  AmjuGL::PushMatrix();
  AmjuGL::Scale(3, 3, 3);
  static float f = 0;
  f += TheTimer::Instance()->GetDt() * 30.0f;
  AmjuGL::RotateX(f);
  AmjuGL::RotateY(f);
  tp.Draw();
  AmjuGL::PopMatrix();
}

void GSEnvMap::OnActive()
{
  GSBase::OnActive();
}

bool GSEnvMap::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSEnvMap::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
