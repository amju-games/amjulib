/*
Amju Games source code (c) Copyright Jason Colman 2000-2012
*/

#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

// Glut-based test for anim classes


#include <AmjuGL.h>
#include <AmjuGL-OpenGL.h>

#include <Md3.h>
#include <Timer.h>

#if defined(MACOSX)
#include <GLUT/Glut.h>
#else
#include <GL/glut.h>
#endif

using namespace Amju;


namespace Amju
{
AmjuGLWindowInfo w(640, 480, false);

bool MyCreateWindowGLUT(AmjuGLWindowInfo*)
{
  return true;
}
}


#define MODEL_PATH  "lara"                              // This stores the folder name the character is in
#define MODEL_NAME      "lara"                          // This stores the prefix for all the character files
#define GUN_NAME    "Railgun"                   // This stores the name of our gun to load

static CModelMD3 md3;
static float yRot = 0;
static float xRot = 0;
static float xPos = 0;
static float yPos = 0; 
static float zPos = 0;
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
  TheTimer::Instance()->Update();

  static bool initialised = false;

  if (!initialised)
  {
#ifdef _DEBUG
std::cout << "Creating mesh...\n";
#endif
    
        // Load the 3 body part meshes and their skin, textures and animation config files
        bool bResult = md3.LoadModel(MODEL_PATH, MODEL_NAME);
        if (!bResult)
        {
            std::cout << "Failed to load model.\n";
        }

        // Load the gun and attach it to our character
        bResult = md3.LoadWeapon(MODEL_PATH, GUN_NAME);
        if (!bResult)
        {
            std::cout << "Failed to load weapon model.\n";
        }

    md3.SetTorsoAnimation("BOTH_DEATH1");
    md3.SetLegsAnimation("LEGS_RUN");
    initialised = true;
  }

  if (!paused)
  {
    //animPlayer.Update(dt, interpolate);
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
      //animPlayer.Update(dt, interpolate);
    }
  }
  
  AmjuGL::InitFrame(); // 0.5f, 0.5f, 0.5f);
  AmjuGL::BeginScene();
  AmjuGL::LookAt(0, 10.0f, 50.0f, 0, 0,0, 0, 1.0f, 0);

  AmjuGL::Translate(xPos, yPos, zPos);

  AmjuGL::RotateX(xRot);
  AmjuGL::RotateY(yRot);

  // Draw origin
  glDisable(GL_TEXTURE_2D);
  glPushAttrib(GL_LIGHTING_BIT);
  glDisable(GL_LIGHTING);
  glColor3f(0, 0, 0);
  glLineWidth(1);
  glBegin(GL_LINES);
  glVertex3f(1, 0, 0);
  glVertex3f(-1, 0, 0);
  glVertex3f(0, 0, 1);
  glVertex3f(0, 0, -1);
  glEnd();
  glColor3f(1, 1, 1);
  glPopAttrib();
  glEnable(GL_TEXTURE_2D);

  md3.DrawModel();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);

  //float t = animPlayer.GetTime();
  //int frame = t * 24.0f;

  //std::string s = "Time: " + ToString(t) + " (" + ToString(frame) + "/24)";
  //PrintText(s, -0.95f, 0.95f);
  PrintText("[M]Mesh [S]Skel [X]Xray [T]Texture [L]Lighting [P]Play/pause [1]Slower [2]Faster", -0.95f, 0.9f);
  PrintText("[Q]Step to next 1/24th sec [0]Reset [I]Interpolate", -0.95f, 0.85f);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);

  AmjuGL::EndScene();

  AmjuGL::Flip();

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
    //animPlayer.ResetTime();
    //animPlayer.Update(0, interpolate);
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
    // Zoom
    zPos += (float)ydiff;
  }
}

int main(int argc, char **argv)
{
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

  AmjuGL::SetImpl(new AmjuGLOpenGL(MyCreateWindowGLUT));
  AmjuGL::CreateWindow(&w);
  AmjuGL::Init();

  float aspect = (float)windowX / (float)windowY;
  AmjuGL::SetPerspectiveProjection(45.0f, aspect, 1.0f, 1000.0f); 

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  glFrontFace(GL_CW); // this means we need to reverse the winding order when we load the tris for an MD3

  glutMainLoop();
  return 0;
}



