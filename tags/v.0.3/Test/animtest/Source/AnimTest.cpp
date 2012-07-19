/*
Amju Games source code (c) Copyright Jason Colman 2000-2011
*/

#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

// Glut-based test for anim classes

/*
static const char* MESH_FILE = "kirk-mesh-uv.txt";
static const char* SKIN_FILE = "kirk-skin.txt";
static const char* SKEL_FILE = "kirk-skel.txt";
static const char* ANIM_FILE = "kirk-anim3.txt";

static const char* MESH_FILE = "dog-mesh.txt";
static const char* SKIN_FILE = "dog-skin.txt";
static const char* SKEL_FILE = "dog-skel.txt";
static const char* ANIM_FILE = "dog-anim.txt";
*/

static const char* MESH_FILE = "amjula-mesh.txt";
static const char* SKIN_FILE = "amjula-skin.txt";
static const char* SKEL_FILE = "amjula-skel.txt";
static const char* ANIM_FILE = "amjula-anim.txt";

#include <AmjuGL.h>
#include <AmjuGL-OpenGL.h>

#include "AnimPlayer.h"
#include "IndexedMesh.h"
#include "Skeleton.h"
#include "Skin.h"
#include "File.h"
#include "StringUtils.h"
#include "AnimManager.h"

#if defined(MACOSX)
#include <GLUT/Glut.h>
#else
#include <GL/glut.h>
#endif

using namespace Amju;

static AnimPlayer animPlayer;
static float yRot = 0;
static float xRot = 0;
static float xPos = 0;
static float yPos = 0;
static bool showSkel = true;
static bool showMesh = true;
static bool xray = false;
static float dt = 0.001f; // Mac default dt 
static bool paused = true;
static bool lighting = true;
static bool textured = true;
static bool stepping = false; // if true, step to next keyframe (1/24 sec)
static float steptime = 0;
static bool interpolate = true;
// Mouse buttons
static int mouseButton[3] = { GLUT_UP, GLUT_UP, GLUT_UP };

void PrintText(const std::string& text, float x, float y)
{
  glPushAttrib(GL_LIGHTING_BIT);
  glDisable(GL_LIGHTING);

  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glRasterPos2f(x, y);
  for (unsigned int i = 0; i < text.size(); i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
  }

  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glPopAttrib();
}

void draw()
{
  static PMesh mesh = 0;
  static bool initialised = false;

  if (!initialised)
  {
#ifdef _DEBUG
std::cout << "Creating mesh...\n";
#endif

/*
    mesh = new IndexedMesh;
    File mf;
    if (!mf.OpenRead(MESH_FILE)) 
    {
      mf.ReportError("No mesh file!\n");
      return;
    }
    if (!mesh->Load(&mf))
    {
      std::cout << "Failed to load mesh!\n";
      return;
    }

    Skeleton* pSkel = new Skeleton;
    File f;
    if (!f.OpenRead(SKEL_FILE)) 
    {
      f.ReportError("No skeleton file!\n");
      return;
    }

    if (!pSkel->Load(&f))
    {
      std::cout << "Failed to load skeleton\n"; 
      return;
    }

    Skin* pSkin = new Skin;
    pSkin->SetSkeleton(pSkel); // needed to look up bone names
    // Load Skin

    File skinf;
    if (!skinf.OpenRead(SKIN_FILE))
    {
      skinf.ReportError("No skin file");
      exit(1);
    }
    if (!pSkin->Load(&skinf))
    {
      std::cout << "Failed to load skin\n";
      exit(1);
    }
    // If no skin is set, mesh is not transformed with bones
    mesh->SetSkin(pSkin);

    animPlayer.CreateBones(pSkel);
    animPlayer.SetMesh(mesh);
*/

    TheAnimManager::Instance()->SetUpAnimPlayer(&animPlayer, MESH_FILE, SKIN_FILE, SKEL_FILE);

    // Load anim - needs skel file for bone names
    if (!TheAnimManager::Instance()->LoadAnim(ANIM_FILE, SKEL_FILE))
    {
      std::cout << "Failed to load anim " << ANIM_FILE << "\n";
      exit(1);
    }

    PAnim pAnim = TheAnimManager::Instance()->GetAnim(ANIM_FILE);

/*
    Anim* pAnim = new Anim;
    pAnim->SetSkeleton(pSkel); // needed to look up bone names

    File af;
    if (!af.OpenRead(ANIM_FILE))
    {
      af.ReportError("No anim file!\n");
      return;
    }
    if (!pAnim->Load(&af))
    {
      std::cout << "Failed to load anim\n";
      return;
    }
*/
    // If no anim, skeleton be will be static
    // Blendtime is ignored for first anim
    animPlayer.SetAnim(pAnim, 0);

    initialised = true;
  }

  if (!paused)
  {
    animPlayer.Update(dt, interpolate);
  }
  else if (stepping)
  {
    steptime += dt;
    if (steptime >= (1.0f/24.0f))
    {
      stepping = false;
      steptime = 0;
    }
    else
    {
      animPlayer.Update(dt, interpolate);
    }
  }
  
  AmjuGL::InitFrame(0.5f, 0.5f, 0.5f);
  AmjuGL::BeginScene();
  AmjuGL::LookAt(0, 10.0f, 20.0f, 0, 0,0, 0, 1.0f, 0);

  AmjuGL::Translate(xPos, yPos, 0);

  AmjuGL::RotateX(xRot);
  AmjuGL::RotateY(yRot);

  // Draw origin
  glDisable(GL_TEXTURE_2D);
  glPushAttrib(GL_LIGHTING_BIT);
  glDisable(GL_LIGHTING);
  glLineWidth(1);
  glBegin(GL_LINES);
  glVertex3f(1, 0, 0);
  glVertex3f(-1, 0, 0);
  glVertex3f(0, 0, 1);
  glVertex3f(0, 0, -1);
  glEnd();
  glPopAttrib();
  glEnable(GL_TEXTURE_2D);

  if (showSkel)
  {
    animPlayer.DrawSkel();
  }

  if (showMesh)
  {
    if (xray)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
    }
    if (lighting)
    {
      glEnable(GL_LIGHTING);
    }
    else
    {
      glDisable(GL_LIGHTING);
    }
    if (textured)
    {
      glEnable(GL_TEXTURE_2D);
    }
    else
    {
      glDisable(GL_TEXTURE_2D);
    }
    animPlayer.Draw();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  }

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);

  float t = animPlayer.GetTime();
  int frame = t * 24.0f;

  std::string s = "Time: " + ToString(t) + " (" + ToString(frame) + "/24)";
  PrintText(s, -0.95f, 0.95f);
  PrintText("[M]Mesh [S]Skel [X]Xray [T]Texture [L]Lighting [P]Play/pause [1]Slower [2]Faster", -0.95f, 0.9f);
  PrintText("[Q]Step to next 1/24th sec [0]Reset [I]Interpolate", -0.95f, 0.85f);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);

  AmjuGL::EndScene();

//#ifndef WIN32
  // WIN32 AmjuGLOpenGL swaps buffers already!
  glutSwapBuffers(); 
//#endif
}

void keydown(unsigned char c, int x, int y)
{
  if (c == 'i' || c == 'I')
  {
    interpolate = !interpolate;
  }
  if (c == 's' || c == 'S')
  {
    showSkel = !showSkel;
  }
  if (c == 'm' || c == 'M')
  {
    showMesh = !showMesh;
  }
  if (c == 'x' || c == 'X')
  {
    xray = !xray;
  }
  if (c == 'l' || c == 'L')
  {
    lighting = !lighting;
  }
  if (c == 't' || c == 'T')
  {
    textured = !textured;
  }
  if (c == 'q' || c == 'Q')
  {
    steptime = 0;
    paused = true;
    stepping = !stepping;
  }

  if (c == 'p' || c == 'P')
  {
    paused = !paused;
  }
  if (c == '1')
  {
    dt *= 0.9f;
  } 
  if (c == '2')
  {
    dt *= 1.1f;
  } 
  if (c == '0')
  {
    animPlayer.ResetTime();
    animPlayer.Update(0, interpolate);
  }

  // TODO 
}

void idle()
{
  glutPostRedisplay();
}

void resize(int w, int h)
{
  glViewport(0, 0, w, h);
}

void mousedown(int button, int state, int x, int y)
{
  mouseButton[button] = state;
}

void mousemove(int x, int y)
{
  static int oldx = x;
  static int oldy = y;
  int xdiff = x - oldx;
  int ydiff = y - oldy;
  oldx = x;
  oldy = y;
  
  if (mouseButton[GLUT_LEFT_BUTTON] == GLUT_DOWN)
  {
    // Rotate
    yRot += (float)xdiff;
    xRot += (float)ydiff;
  }
  else if (mouseButton[GLUT_MIDDLE_BUTTON] == GLUT_DOWN)
  {
    // Pan
    yPos -= (float)ydiff; // screen y-coord is "upside-down"
    xPos += (float)xdiff;
  }
  else if (mouseButton[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
  {
  }
}

int main(int argc, char **argv)
{
  AmjuGL::SetImpl(new AmjuGLOpenGL);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  int windowX = 640;
  int windowY = 480;
  glutInitWindowSize(windowX, windowY);
  glutCreateWindow("Anim Test");

  // Set callbacks
  glutDisplayFunc(draw);
  glutIdleFunc(idle);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keydown);
  //glutKeyboardUpFunc(keyup);
  //glutSpecialFunc(specialkeydown);
  //glutSpecialUpFunc(specialkeyup);
  glutMouseFunc(mousedown);
  glutMotionFunc(mousemove);
  glutPassiveMotionFunc(mousemove);


  AmjuGL::Init();
  float aspect = (float)windowX / (float)windowY;
  AmjuGL::SetPerspectiveProjection(45.0f, aspect, 1.0f, 1000.0f); // why not just use gluPerspective?

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  glutMainLoop();
  return 0;
}



