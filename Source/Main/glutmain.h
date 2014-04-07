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
#include <StringUtils.h>

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

#ifdef WIN32
  glutForceJoystickFunc();
#endif

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
  else if (k == ' ')
  {
    ke->keyType = AMJU_KEY_SPACE;
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
  static int oldx = -1;
  static int oldy = -1;
  if (x == oldx && y == oldy)
  {
    return; // GLUT generates a lot of no op mouse events ???
  }
  oldx = x;
  oldy = y;

  CursorEvent* ce = new CursorEvent;
  ce->controller = 0;
  ce->x = (float)x / (float)Amju::Screen::X() * 2.0f - 1.0f; 
  ce->y = 1.0f - (float)y / (float)Amju::Screen::Y() * 2.0f; 
  QueueEvent(ce);
}

void QueueButtonEvent(Button b, int controller, bool isDown)
{
  ButtonEvent* be = new ButtonEvent;
  be->controller = controller; 
  be->button = b;
  be->isDown = isDown;
  QueueEvent(be);

static const char* BUTTON_NAME[8] = { "A", "B", "C", "D", "Left", "Right", "Up", "Down" };
std::cout << "Joystick " << BUTTON_NAME[(int)b] << "  button " << (be->isDown ? "down" : "up") << "\n";
}

void joystick(unsigned int buttonMask, int x, int y, int z)
{
  static int oldx = -99999;
  static int oldy = -99999;
  static int oldz = -99999;
  static unsigned int oldbuttons = buttonMask; // this is OK, first event is bogus anyway

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

    int controller = 0; // TODO only one joystick in GLUT ?
    if (diff & 0x01)
    {
      QueueButtonEvent(AMJU_BUTTON_A, controller, (buttonMask & 0x01) != 0); 
    }
    if (diff & 0x02)
    {
      QueueButtonEvent(AMJU_BUTTON_B, controller, (buttonMask & 0x02) != 0); 
    }
    if (diff & 0x04)
    {
      QueueButtonEvent(AMJU_BUTTON_C, controller, (buttonMask & 0x04) != 0); 
    }
    if (diff & 0x08)
    {
      QueueButtonEvent(AMJU_BUTTON_D, controller, (buttonMask & 0x08) != 0); 
    }
    // ... TODO 

    oldbuttons = buttonMask;
  }
}

namespace Amju
{
extern AmjuGLWindowInfo w;

// TODO Make this a GLUT version in AmjuGLOpenGL
bool CreateWindowGLUT(AmjuGLWindowInfo* w)
{
  bool windowed = true;
  if (w->IsFullScreen())
  {
#ifdef GLUT_GAME_MODE_WORKS // crashes on Mac, ruins desktop on Windows

    std::string str = ToString(w->GetWidth()) + "x" + ToString(w->GetHeight()) + ":32";
    glutGameModeString(str.c_str());
    // enter full screen
    if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
    {
std::cout << "Glut entering game mode..\n";
      glutEnterGameMode();
      windowed = false;
    }
    else
    {
std::cout << "Glut can't enter game mode..\n";
    }

#endif // GLUT_GAME_MODE_WORKS
  }

  if (windowed)
  {
std::cout << "Glut creating window...\n";
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

#ifdef WIN32
  glutJoystickFunc(joystick, 0); // we will force polling
#else
  glutJoystickFunc(joystick, 50);
#endif

  return true;
}
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

  TheEventPoller::Instance()->SetImpl(new EventPollerImpl); 

  AmjuGL::SetImpl(new AmjuGLOpenGL(CreateWindowGLUT));

  // This game-specific function must be defined for each project
  StartUpBeforeCreateWindow();

  // Defined in game-specific code
  Amju::AmjuGL::CreateWindow(&w);
  Amju::AmjuGL::Init();

  StartUpAfterCreateWindow();

  // Can't do this, glutMainLoop is in charge
  //TheGame::Instance()->Run();

  glutMainLoop();

  return 0;
}
 
#endif // AMJU_USE_GLUT

