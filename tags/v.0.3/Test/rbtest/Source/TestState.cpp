#include <iostream>
#include <fstream>
#include "TestState.h"
#include "ObjMesh.h"
#include "Game.h"
#include "Directory.h"
#include "StringUtils.h"
#include "Font.h"
#include "SceneMesh.h"
#include "File.h"
#include "RBBox3.h"
#include "RBSphere3.h"
#include "Screen.h"

namespace Amju
{
void TestState::OnActive()
{
  GameState::OnActive();
  
}

void TestState::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 1, 1));

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR = 1.0f; 
  const float FAR = 3000.0f;
  float aspect = (float) Screen::X() / (float) Screen::Y();
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR, FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::LookAt(0, 0, 20,   0, 0, 0.0f,  0, 1.0f, 0);

  static RBBox3 rb;
  static RBSphere3 rs;
  static bool first = true;
  if (first)
  {
    first = false;
    rb.SetPos(Vec3f(0, 4, 0));
    rs.SetPos(Vec3f(3, 4, 0));

    //rb.AddForce(Vec3f(0, -10, 0)); // one-time acc, should give constant vel
    //rb.AddTorque(Vec3f(0, -4, 0), Vec3f(2, 1, 0));
    //rb.AddTorque(Vec3f(0, 10, 0), Vec3f(-2, 1, 0));
  }
  rb.AddForce(Vec3f(0, -1, 0)); // every frame - gravity
  rb.AddTorque(Vec3f(0, -4, 0), Vec3f(2, 1, 0));

  rs.AddForce(Vec3f(0, -1, 0)); // every frame - gravity
  rb.Update();
  rb.Draw();
  rs.Update();
  rs.Draw();
  //GetGameSceneGraph()->Update();
  //GetGameSceneGraph()->Draw();

  /*
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(1, 1, 1)); // Light direction

  AmjuGL::SetTextureMode(AmjuGL::AMJU_TEXTURE_SPHERE_MAP);

  static float a = 0;
  AmjuGL::RotateX(a);
  a += 0.2f;
  TheSceneGraph::Instance()->Draw();

  AmjuGL::SetTextureMode(AmjuGL::AMJU_TEXTURE_REGULAR);

  // Scaling breaks lighting because it fucks the normals ?????
//  AmjuGL::Scale(100, 100, 100);
 // m1.Draw();
//  m1.GetAABB.Draw();

//  m1.Draw();
*/
}

void TestState::Draw2d()
{
}
}
