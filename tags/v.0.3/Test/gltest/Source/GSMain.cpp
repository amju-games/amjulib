#include "GsMain.h"
#include <AmjuGL.h>
#include <AABB.h>
#include <DrawAABB.h>
#include <Shader.h>
#include <Matrix.h>
#include "Grid.h"
#include "Teapot.h"
#include <Colour.h>
#include <Vec2.h>
#include "GSFinish.h"
#include <AlphaBmpLoader.h>
#include <Font.h>
#include <Screen.h>

namespace Amju
{
static Shader* shader = 0;

Grid grid;

GSMain::GSMain()
{
  m_tex = 0;
}

void GSMain::Update()
{
}

void GSMain::Draw()
{
  GSBase::Draw();

  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));

  AmjuGL::SetColour(1, 1, 1, 1);

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(1, 1, 1)); // Light direction

  //AmjuGL::Translate(3.0f, 0, 0);
  // Should be same as:
  /*
  Matrix mat;
  mat.Translate(Vec3f(3.0f, 0, 0));
  AmjuGL::MultMatrix(mat);
  */

  static float f = 0;
  f += 0.2f;
  AmjuGL::RotateY(f);
  //AmjuGL::RotateX(f);

  AABB aabb(-1, 1, -1, 1, -1, 1);
  m_tex->UseThisTexture(); // not required, we set texture in shader

  //DrawSolidAABB(aabb);

  static Teapot tp(2.0f);
  //tp.Draw();
  //return;

  // Use Shader: we get the modelview and projection matrices, and set them
  //  in the shader. So any transforms after we have set them will have
  //  no effect!

  Matrix matrix;
  Matrix mview;
  mview.ModelView();

  shader->Set("gWorld", mview);

  Matrix proj;
  proj.Projection();

  matrix = mview * proj;

  shader->Set("gWVP", matrix);

  // Get inverse transpose of world (mview) matrix, for correct
  //  transformation of normals
  Matrix mviewInverseTranspose = mview;
  mviewInverseTranspose.Inverse();
  mviewInverseTranspose = Transpose(mviewInverseTranspose);
  shader->Set("gWorldInverseTranspose", mviewInverseTranspose);

  // Light direction
  shader->Set("gLightVecW", AmjuGL::Vec3(1, 1, 1));

  // Eye pos - from lookAt 
  shader->Set("gEyePosW", AmjuGL::Vec3(0, 5.0f, 10.0f));

  shader->Set("gDiffuseMtrl", Colour(1, 1, 1, 1));
  shader->Set("gDiffuseLight", Colour(1, 1, 1, 1));

  shader->Set("gAmbientMtrl", Colour(0, 0, 0, 1));
  shader->Set("gAmbientLight", Colour(1, 1, 1, 1));

  shader->Set("gSpecularMtrl", Colour(1, 1, 1, 1));
  shader->Set("gSpecularLight", Colour(1, 1, 1, 1));
//  shader->Set("gSpecularPower", 100.0f);
  
  shader->Begin(); 

  static float myInput = 0;
  myInput += 0.01f;
  shader->Set("gTime", myInput);
  

  shader->Set("gTex", m_tex->GetId());

  shader->Begin(); //"myTechnique");
  //int numPasses = shader->GetNumPasses();
  //shader->BeginPass(0); // TODO Assume 1 pass for now

  //DrawSolidAABB(aabb);
  //grid.Draw();
  // NB No transforms will have any effect here
  tp.Draw();

  //shader->EndPass();
  shader->End();
}

void GSMain::Draw2d()
{
//  AmjuGL::Viewport(0, 0, Screen::X(), Screen::Y());
//  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
//  AmjuGL::SetIdentity();
//  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
//  AmjuGL::SetIdentity();

  static Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");
  Assert(font);
  font->Print(-1, -0.8f, "Hello?");
//  return;
  
  PGuiElement gui = LoadGui("finish-gui.txt");
  Assert(gui);
  gui->Draw();

  m_tex->UseThisTexture(); 

  // Draw a GUI Image
  Vec2f pos(0, 0);
  Vec2f size(1.0f, 0.5f);

  float z = 0.5f;
  AmjuGL::Tris tris;
  AmjuGL::Vert verts[4] = 
  {
    AmjuGL::Vert(pos.x + size.x, pos.y - size.y, z,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
    AmjuGL::Vert(pos.x + size.x, pos.y, z,   1, 1,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y, z,   0, 1,   0, 1, 0),
    AmjuGL::Vert(pos.x, pos.y - size.y, z,   0, 0,   0, 1, 0)
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

  AmjuGL::DrawTriList(tris);
}

void GSMain::OnActive()
{
	TheResourceManager::Instance()->AddLoader("bmpa", BmpALoader);
  TheResourceManager::Instance()->AddLoader("font", FontLoader);
  TheResourceManager::Instance()->LoadResourceGroup("2dtext-group");

//  TheGame::Instance()->SetCurrentState(GSFinish::NAME); 

  shader = AmjuGL::LoadShader("Shaders/simplest");
//  shader = AmjuGL::LoadShader("Shaders/wavedemo");

  // TODO Lighting - specular doesn't seem to be controllable
  // Textures - bad bits, mipmap issue ?
  //  OK on XPS, so maybe driver issue

  //shader->Load("simplest.fx");
  //shader->Load("wavedemo.fx");
  //shader->Load("lighting.fx");
  //shader->Load("phong.fx");
  //shader->Load("texturing.fx");
  //shader->Load("texturing-gencoords.fx"); // generates spherical tex coords
  //shader->Load("phong-texturing-gencoords.fx"); // phong + generates spherical tex coords
  //shader->Load("spheremap.fx");
//  shader->Load("jelly+spheremap.fx"); // need to calc normals
  // TODO : 
  //  toon
  //  outlined
  //  colouredWaves - wavedemo with lighting, calc normals
  //  mandelbrot!?
  //  bump map, normal map
  //  fire
  //  shadow mapping
  //  animation: calc vertex transforms!

  GameState::OnActive();
  static const char* TEX = "spheremap_bar.bmp";
  //static const char* TEX = "bw.bmp";
  //static const char* TEX = "font2d/arial_512_16pt.bmpa";

  m_tex = (Texture*)TheResourceManager::Instance()->GetRes(TEX);

  grid.Build(100, 20.0f);
}
}
