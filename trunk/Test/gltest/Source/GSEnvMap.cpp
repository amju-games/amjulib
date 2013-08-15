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
}

void GSEnvMap::Update()
{
  GSBase::Update();
}

void GSEnvMap::Draw()
{
  AmjuGL::SetClearColour(Colour(0.2f, 0.2f, 0.2f, 1));
  GSBase::Draw();

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

void GSEnvMap::Draw2d()
{
  GSBase::Draw2d();
}

void GSEnvMap::OnActive()
{
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
