/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#ifndef OPEN_GL_H_INCLUDED
#define OPEN_GL_H_INCLUDED

// Workarounds for non-standard Microsoft gl.h, and non-standard
// Mac OpenGL diectory.

#if defined(MACOSX)
#include "GL/glew.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#endif // MACOSX

#ifdef WIN32
#include <windows.h>
#ifndef GLEW_STATIC
#error Dude, define GLEW_STATIC in the project settings.
#endif
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#endif // WIN32

#ifdef IPHONE
#include <OpenGLES/ES1/gl.h>
#endif // IPHONE

#endif // Include guard 



