#include <AmjuGL.h>
#include <Vec2.h>
#include <DrawAABB.h>
#include <ObjMesh.h>
#include <ResourceManager.h>
#include <Shader.h>
#include "GSMandel.h"
#include "GSTexture.h"

namespace Amju
{
static Shader* shader = 0;

static Vec2f mousepos;
static bool drag = false;

GSMandel::GSMandel()
{
  m_name = "Mandelbrot";
  m_nextState = TheGSTexture::Instance();
  m_maxTime = 10.0f;
}

bool GSMandel::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  drag = mbe.isDown;
  return true;
}

bool GSMandel::OnCursorEvent(const CursorEvent& ce)
{
  if (drag)
  {
    Vec2f v(ce.dx, ce.dy);
    v *= (1.0f / (1.0f + m_time));
    mousepos += v;
//std::cout << mousepos.x << ", " << mousepos.y << "\n";

  }
  return true;
}

void GSMandel::Update()
{
   GSBase::Update();
}

void GSMandel::DrawScene()
{
//  DrawHelp();
  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));

  static ObjMesh* sphere = 
    (ObjMesh*)TheResourceManager::Instance()->GetRes("mandel_sphere.obj");

  shader->Begin();

  float t = m_time; // * m_time * m_time; // ?
  shader->Set("gTime", t);
  shader->Set("mouseX", mousepos.x);
  shader->Set("mouseY", mousepos.y);
  float mi = 100 + 10 * m_time * m_time;
//std::cout << mi << "\n";
  shader->Set("maxIters", mi);

//  sphere->Draw();

  AmjuGL::Draw(m_triList);

  shader->End();
}

void GSMandel::OnActive()
{
  GSBase::OnActive();

  shader = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/mandel");
  shader->Begin();
  
  // Make a rectangle in screen space 
  Vec2f pos(-0.5f, 0.5f);
  Vec2f size(1.0f, 1.0f);

  float z = 0.5f;
  AmjuGL::Tris tris;
  const float XMIN = -2;
  const float XMAX = 2;
  const float YMAX = 2;
  const float YMIN = -2;
  AmjuGL::Vert verts[4] = 
  {
    AmjuGL::Vert(pos.x + size.x, pos.y - size.y, z,   XMAX, YMIN,   0, 1, 0), 
    AmjuGL::Vert(pos.x + size.x, pos.y, z,   XMAX, YMAX,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y, z,   XMIN, YMAX,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y - size.y, z,   XMIN, YMIN,   0, 1, 0)
  };

  tris.clear();
  tris.reserve(2);
  AmjuGL::Tri tri;
  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[1];
  tri.m_verts[2] = verts[2];
  tris.push_back(tri);

  tri.m_verts[0] = verts[0];
  tri.m_verts[1] = verts[2];
  tri.m_verts[2] = verts[3];
  tris.push_back(tri);

  m_triList = MakeTriList(tris);
}

} // namespace
