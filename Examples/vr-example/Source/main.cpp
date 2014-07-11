#ifdef WIN32
// Windows: use OpenGL or Direct 3D - can choose at runtime, 
// so don't make this a compile time switch ?
// OR: allow compile-time switch to completely remove DX or OpenGL, 
// reducing DLL dependencies

// Define one of these - this chooses the API at compile time ATM
#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT
//#define AMJU_USE_DX9
//#define AMJU_USE_DX11

#endif // WIN32

#ifdef MACOSX
#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT
#endif

#if !defined(AMJU_IOS) && !defined(ANDROID_NDK)
#include <main.h>
#endif


