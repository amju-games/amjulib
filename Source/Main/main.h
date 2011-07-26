namespace Amju
{
void StartUp();
}

#ifdef IPHONE
#include "iPhone/Classes/EAGLView.mm"
#include "iPhone/Classes/ES1Renderer.mm"
#include "iPhone/Classes/iPhoneAppDelegate.mm"
#include "iPhone/main.m"
#else // IPHONE

#ifdef AMJU_USE_GLUT
#include <glutmain.h>
#else // AMJU_USE_GLUT
#include <nonglutmain.h>
#endif // AMJU_USE_GLUT

#endif // IPHONE

