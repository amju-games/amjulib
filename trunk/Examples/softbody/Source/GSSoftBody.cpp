#include <ResourceManager.h>
#include "GSSoftbody.h"
#include "Squishy.h"

namespace Amju
{
Squishy sq;

GSSoftBody::GSSoftBody()
{
}

void GSSoftBody::Update()
{
  sq.Update();
}

void GSSoftBody::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 1, 1));
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetPerspectiveProjection(45.0f, 1.3f, 1.0f, 1000.0f); //FOVY, ASPECT, NEAR, FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::LookAt(50, 0, 50,  0, 0, 0,   0, 1, 0);

  Vec3f pos(1, 1, 1);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(pos.x, pos.y, pos.z)); // Light direction

  AmjuGL::Translate(0, -10, 0);
  sq.Draw();
}

void GSSoftBody::Draw2d()
{
}

void GSSoftBody::OnActive()
{
  // K value is how soft/tough
  const float K = 1.0f; // TODO

  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes("bean.obj");
  if (!mesh)
  {
    std::cout << "Failed to load obj mesh!\n";
    Assert(0);
  }

  sq.Init(mesh, K);
}
}
