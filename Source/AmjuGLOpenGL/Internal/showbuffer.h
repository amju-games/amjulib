// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#ifndef SHOWBUFFER_H
#define SHOWBUFFER_H


#include "OpenGL.h"



extern void
ShowDepthBuffer( GLsizei winWidth, GLsizei winHeight,
                 GLfloat zBlack, GLfloat zWhite );


extern void
ShowAlphaBuffer( GLsizei winWidth, GLsizei winHeight );


extern void
ShowStencilBuffer( GLsizei winWidth, GLsizei winHeight,
                   GLfloat scale, GLfloat bias );



#endif
