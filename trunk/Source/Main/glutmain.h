#ifdef AMJU_USE_GLUT
#include <AmjuFirst.h>
#include <iostream>

#include <AmjuGL-OpenGL.h>
#include <Screen.h>
#include <Game.h>
#include <SoundManager.h>
#include <BassSoundPlayer.h>
#include <AmjuGLWindowInfo.h>
#include <EventPoller.h>

// TODO include glut.h for other platforms
#ifdef MACOSX
#include <GLUT/glut.h>
#endif
#ifdef WIN32
#include <gl/glut.h>
#endif

#include <AmjuFinal.h>

//#define KEY_CODE_DEBUG

using namespace Amju;


void QueueEvent(Amju::Event* e)
{
  Amju::TheEventPoller::Instance()->GetImpl()->QueueEvent(e);
}

void resize(int x, int y)
{
//std::cout << "GLUT resize callback, x: " << x << " y: " << y << "\n";
  Screen::SetSize(x, y);
  ResizeEvent* e = new ResizeEvent;
  e->type = AMJU_RESIZE; // TODO How do we get minimise, maximise etc
  e->x = x;
  e->y = y;
  QueueEvent(e);
}

void draw()
{
  TheGame::Instance()->RunOneLoop();

#ifndef WIN32
  glutSwapBuffers(); // ?
#endif

  glutPostRedisplay();
}

void key(char k, bool down)
{
  KeyEvent* ke = new KeyEvent;  
  ke->modifier = glutGetModifiers();
  ke->keyDown = down;

  if (k == 127) // backspace
  {
#ifdef WIN32
    ke->keyType = AMJU_KEY_DELETE;
#else
    ke->keyType = AMJU_KEY_BACKSPACE;
#endif
  }
  else if (k == 8) // delete
  {
#ifdef WIN32
    ke->keyType = AMJU_KEY_BACKSPACE;
#else
    ke->keyType = AMJU_KEY_DELETE;
#endif
  }
  else if (k == 13)
  {
    ke->keyType = AMJU_KEY_ENTER;
  }
  else if (k == 27) // esc
  {
    ke->keyType = AMJU_KEY_ESC;
  }
  else
  {
#ifdef KEY_CODE_DEBUG
std::cout << "Got key " << (down ? "DOWN" : "UP") << " event, char is : " << k << " (" << (int)k << ")\n";
#endif

    ke->keyType = AMJU_KEY_CHAR;
    ke->key = k;
  }

  QueueEvent(ke);
}

void keydown(unsigned char k, int, int)
{
  key(k, true);
}

void keyup(unsigned char k, int, int)
{
  key(k, false);
}

void special(int key, bool down)
{
  KeyEvent* ke = new KeyEvent;  

  Assert(GLUT_ACTIVE_SHIFT == AMJU_KEY_MOD_SHIFT);
  Assert(GLUT_ACTIVE_CTRL == AMJU_KEY_MOD_CTRL);
  Assert(GLUT_ACTIVE_ALT == AMJU_KEY_MOD_ALT);

  ke->modifier = glutGetModifiers();
  switch (key)
  {
  case GLUT_KEY_LEFT:
    ke->keyType = AMJU_KEY_LEFT;
    break;

  case GLUT_KEY_RIGHT:
    ke->keyType = AMJU_KEY_RIGHT;
    break;

  case GLUT_KEY_UP:
    ke->keyType = AMJU_KEY_UP;
    break;

  case GLUT_KEY_DOWN:
    ke->keyType = AMJU_KEY_DOWN;
    break;

  case GLUT_KEY_PAGE_UP:
    ke->keyType = AMJU_KEY_PAGE_UP;
    break;

  case GLUT_KEY_PAGE_DOWN:
    ke->keyType = AMJU_KEY_PAGE_DOWN;
    break;

  case GLUT_KEY_HOME:
    ke->keyType = AMJU_KEY_HOME;
    break;

  case GLUT_KEY_END:
    ke->keyType = AMJU_KEY_END;
    break;
  }
  ke->keyDown = down;
  QueueEvent(ke);
}

void specialkeydown(int key, int, int)
{
  special(key, true);
}

void specialkeyup(int key, int, int)
{
  special(key, false);
}

void mousebutton(int button, int state, int x, int y)
{
  MouseButtonEvent* mbe = new MouseButtonEvent;
  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    mbe->button = AMJU_BUTTON_MOUSE_LEFT;
    break;

  case GLUT_RIGHT_BUTTON:
    mbe->button = AMJU_BUTTON_MOUSE_RIGHT;
    break;

  case GLUT_MIDDLE_BUTTON:
    mbe->button = AMJU_BUTTON_MOUSE_MIDDLE;
    break;
  }
  mbe->isDown = (state == GLUT_DOWN);
  mbe->x = (float)x / (float)Amju::Screen::X() * 2.0f - 1.0f; 
  mbe->y = 1.0f - (float)y / (float)Amju::Screen::Y() * 2.0f; 
  QueueEvent(mbe);
}

void mousemove(int x, int y)
{
  CursorEvent* ce = new CursorEvent;
  ce->controller = 0;
  ce->x = (float)x / (float)Amju::Screen::X() * 2.0f - 1.0f; 
  ce->y = 1.0f - (float)y / (float)Amju::Screen::Y() * 2.0f; 
  QueueEvent(ce);
}

void joystick(unsigned int buttonMask, int x, int y, int z)
{
  // TODO ignore if same state as last time
  static int oldx = -99999;
  static int oldy = -99999;
  static int oldz = -99999;
  static unsigned int oldbuttons = 99999;

  if (x != oldx || y != oldy || z != oldz)
  {
    oldx = x;
    oldy = y;
    oldz = z;

    JoyAxisEvent* j = new JoyAxisEvent;
    j->controller = 0;
    // TODO What is the min/max value ??
    j->x = (float)x / 2000.0f;
    j->y = -(float)y / 2000.0f;
    QueueEvent(j);
  }

  if (oldbuttons != buttonMask)
  {
    // TODO buttons
std::cout << "Button mask: " << buttonMask << "\n";

    unsigned int diff = oldbuttons ^ buttonMask;
std::cout << "Diff: " << diff << "\n";

    ButtonEvent* be = new ButtonEvent;
    be->controller = 0; // TODO only one joystick in GLUT ?
    if (diff & 0x01)
    {
      be->button = AMJU_BUTTON_A;
      be->isDown = buttonMask & 0x01; 
    }
    // ... TODO 

    oldbuttons = buttonMask;
    QueueEvent(be);
  }
}

namespace Amju
{
extern AmjuGLWindowInfo w;

// TODO Make this a GLUT version in AmjuGLOpenGL
bool CreateWindowGLUT(AmjuGLWindowInfo* w)
{
  // w is global defined in game-specific code
  glutInitWindowSize(w->GetWidth(), w->GetHeight()); 

  if (w->IsFullScreen())
  {
    // TODO glutGameMode
  }
  else
  {
    // w is global defined in game-specific code
    glutInitWindowSize(w->GetWidth(), w->GetHeight()); 
    glutCreateWindow(w->GetTitle()); 
  }
 
  // TODO Set icon

  glutDisplayFunc(draw);
  glutReshapeFunc(resize);

  // DON'T auto-repeat - we will do it ourselves
  glutIgnoreKeyRepeat(1);

  glutKeyboardFunc(keydown);
  glutKeyboardUpFunc(keyup);
  glutSpecialFunc(specialkeydown);
  glutSpecialUpFunc(specialkeyup);
  glutMouseFunc(mousebutton);
  glutMotionFunc(mousemove);
  glutPassiveMotionFunc(mousemove);

  glutIdleFunc(draw);

  int js = glutDeviceGet(GLUT_HAS_JOYSTICK);
  if (js != 0)
  {
    std::cout << "GLUT says joystick available!\n";
  }
  glutJoystickFunc(joystick, 50);
  return true;
}
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

  TheEventPoller::Instance()->SetImpl(new EventPollerImpl); 

  AmjuGL::SetImpl(new AmjuGLOpenGL(CreateWindowGLUT));

  // Defined in game-specific code
  Amju::AmjuGL::CreateWindow(&w);
  Amju::AmjuGL::Init();

  // This game-specific function must be defined for each project
  StartUp();

  // Can't do this, glutMainLoop is in charge
  //TheGame::Instance()->Run();

  glutMainLoop();

  return 0;
}
 
#endif // AMJU_USE_GLUT

