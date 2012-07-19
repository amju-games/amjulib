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
#ifdef AMJU_USE_GLUT
#include <GLUT/glut.h>
#endif
#endif // MACOSX

#ifdef WIN32
#include <windows.h>
#ifndef GLEW_STATIC
#error Dude, define GLEW_STATIC in the project settings.
#endif
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#ifdef AMJU_USE_GLUT
#include <gl/glut.h>
#endif
#endif // WIN32

#ifdef IPHONE

#ifdef AMJU_USE_ES2
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#else // AMJU_USE_ES2
#include <OpenGLES/ES1/gl.h>
#endif // AMJU_USE_ES2

#endif // IPHONE

#endif // Include guard 



