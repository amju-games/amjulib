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

#if !defined(AMJU_IOS) && !defined(ANDROID_NDK) && !defined(AMJU_USE_ES2)
#include <main.h>
#endif

#if defined(WIN32)
#if defined(AMJU_USE_ES2)
// TODO: Win main which creates EGL surface. See PowerVR example etc.
#pragma comment(lib, "../../../../../Build/ES2Debug/AmjuLibMsvc.lib")
#else
#ifdef _DEBUG
#pragma comment(lib, "../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../Build/Release/AmjuLibMsvc.lib")
#endif // _DEBUG
#endif // AMJU_USE_ES2
#endif


