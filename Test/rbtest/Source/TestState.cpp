#include <iostream>
#include <fstream>
#include "TestState.h"
#include "ObjMesh.h"
#include "Game.h"
#include "Directory.h"
#include "StringUtils.h"
#include "CursorManager.h"
#include "Font.h"
#include "MySceneGraph.h"
#include "SceneMesh.h"
#include "BonusParticleEffect.h"
#include "File.h"
#include "RBBox.h"
#include "RBSphere.h"
#include "Screen.h"

namespace Amju
{
// TODO Used by Directory code
const char* APPLICATION_NAME = "balance";

const char* TestState::NAME = "test-state";

void TestState::OnActive()
{
 //  LeafLoad("star.leaf"); 
/*  
  // Merge 2 obj files into one
  //for (char c = 'a'; c <= 'z'; c++)
  for (char c = '0'; c <= '9'; c++)
  {
    ObjMesh m1;
    m1.Load("font3d/" + std::string(1, c) + "1.obj");

    ObjMesh m2;
    m2.Load("font3d/" + std::string(1, c) + "2.obj");
    m1.Merge(m2);

    m1.Save("font3d/_" + std::string(1, c) + "_.obj");
  }
  exit(0);
*/

  GameState::OnActive();
  
  TheResourceManager::Instance()->LoadResourceGroup("2dtext-group");
  TheResourceManager::Instance()->LoadResourceGroup("level-1-group");

  /*
  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes("heart.obj");
  SceneMesh* node = new SceneMesh;
  node->SetMesh(mesh);
  node->SetBlended(true);
  */

  BonusParticleEffect* node = new BonusParticleEffect;
  File effectFile;
  if (!effectFile.OpenRead("bonus-effect.txt"))
  {
    effectFile.ReportError("Couldn't open bonus effect file");
    Assert(0);
  }
  node->Load(&effectFile);

  GetGameSceneGraph()->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);
  GetGameSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->AddChild(node);
  Matrix mat;
  mat.Translate(Vec3f(50, 0, 0));
  node->SetLocalTransform(mat);
  node->Start();

  /*
  TheSceneGraph::Instance(); // make sure we have registered with factory
  PSceneNode scene = LoadScene("scene.txt");
  TheSceneGraph::Instance()->SetRootNode(SceneGraph::AMJU_OPAQUE, scene);
  */
  
/*
  // Get directory, load each file in turn
  std::vector<std::string> files;
  Dir("font3d/", &files, false);
  for (unsigned int i = 0; i < files.size(); i++)
  {
    const std::string& s = files[i];
    if (GetFileExt(s) == "leaf")
    {
      LeafLoad(s); 
    }
  }
*/  
}

void TestState::Draw()
{
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

  static RBBox rb;
  static RBSphere rs;
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
//  Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/cheri-font.font");
  Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");

  //font->Print(-1, 0.5,  "abcdefghijklmnopqrstuvwxyz");
  //font->Print(-1, 0,    "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  //font->Print(-1, -0.5, "!\"£$%^&*()_-+=[{}#~:;@'<,>.?/");
  

  TheCursorManager::Instance()->Draw();
}
}
