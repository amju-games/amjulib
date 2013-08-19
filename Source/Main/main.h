namespace Amju
{
void StartUpBeforeCreateWindow();
void StartUpAfterCreateWindow();
}

#ifdef IPHONE

#ifdef AMJU_CONSOLE
#include <nonglutmain.h>
#else

#include "iPhone/Classes/EAGLView.mm"
#include "iPhone/Classes/ES1Renderer.mm"
#include "iPhone/Classes/iPhoneAppDelegate.mm"
#include "iPhone/main.mm"

#endif // AMJU_CONSOLE
#else // IPHONE

#ifdef AMJU_CONSOLE
#include <nonglutmain.h>
#else

#ifdef AMJU_USE_GLUT
#include <glutmain.h>
#else // AMJU_USE_GLUT
#include <nonglutmain.h>
#endif // AMJU_USE_GLUT

#endif // AMJU_CONSOLE

#endif // IPHONE

