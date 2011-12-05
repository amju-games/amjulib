#include "ShadowMapOpenGL1.h"
#define AMJU_USE_GLUT
#include <AmjuGL.h> // TODO Can merrily use raw OpenGL here
#include <Screen.h>
#include "OpenGL.h"
#include <AmjuAssert.h>
#include <iostream>
#include <math.h>

extern "C"
{
#include "showbuffer.h"
}

// Uses demo code from http://www.udel.edu/CIS/software/dist/Mesa-6.4.2/progs/demos/
/*
 * Shadow demo using the GL_ARB_depth_texture, GL_ARB_shadow and
 * GL_ARB_shadow_ambient extensions (or the old SGIX extensions).
 *
 * Brian Paul
 * 19 Feb 2001
 *
 * Added GL_EXT_shadow_funcs support on 23 March 2002
 *
 * Copyright (C) 1999-2001  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */



namespace Amju
{
static GLboolean HaveEXTshadowFuncs = GL_FALSE;

static void
Init(void)
{
#if defined(GL_ARB_depth_texture) && defined(GL_ARB_shadow)
   if (!glutExtensionSupported("GL_ARB_depth_texture") ||
       !glutExtensionSupported("GL_ARB_shadow")) {
      printf("Sorry, this demo requires the GL_ARB_depth_texture and GL_ARB_shadow extensions\n");
      exit(1);
   }
   printf("Using GL_ARB_depth_texture and GL_ARB_shadow\n");
#elif defined(GL_SGIX_depth_texture) && defined(GL_SGIX_shadow)
   if (!glutExtensionSupported("GL_SGIX_depth_texture") ||
       !glutExtensionSupported("GL_SGIX_shadow")) {
      printf("Sorry, this demo requires the GL_SGIX_depth_texture and GL_SGIX_shadow extensions\n");
      exit(1);
   }
   printf("Using GL_SGIX_depth_texture and GL_SGIX_shadow\n");
#endif
   HaveEXTshadowFuncs = glutExtensionSupported("GL_EXT_shadow_funcs");

   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#if defined(GL_ARB_shadow)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
                   GL_COMPARE_R_TO_TEXTURE_ARB);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
#elif defined(GL_SGIX_shadow)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_OPERATOR_SGIX,
                   GL_TEXTURE_LEQUAL_R_SGIX);
#endif

#if defined(GL_ARB_shadow_ambient)
   if (glutExtensionSupported("GL_ARB_shadow_ambient")) {
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, 0.3);
      printf("and GL_ARB_shadow_ambient\n");
   }
#elif defined(GL_SGIX_shadow_ambient)
   if (glutExtensionSupported("GL_SGIX_shadow_ambient")) {
      glTexParameterf(GL_TEXTURE_2D, GL_SHADOW_AMBIENT_SGIX, 0.3);
      printf("and GL_SGIX_shadow_ambient\n");
   }
#endif

   /* setup 1-D grayscale texture image for SHOW_DISTANCE mode */
   {
      GLuint i;
      GLubyte image[256];
      for (i = 0; i < 256; i++)
         image[i] = i;
      glTexImage1D(GL_TEXTURE_1D, 0, GL_LUMINANCE,
                   256, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
   }

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}




bool ShadowMapOpenGL1::Init()
{
  Amju::Init();

  return true;
}


#define DEG_TO_RAD (3.14159 / 180.0)

//static GLint WindowWidth = 450, WindowHeight = 300;
//static GLfloat Xrot = 15, Yrot = 0, Zrot = 0;

//static GLfloat Red[4] = {1, 0, 0, 1};
//static GLfloat Green[4] = {0, 1, 0, 1};
//static GLfloat Blue[4] = {0, 0, 1, 1};
//static GLfloat Yellow[4] = {1, 1, 0, 1};

static GLfloat LightDist = 10;
static GLfloat LightLatitude = 45.0;
static GLfloat LightLongitude = 45.0;
static GLfloat LightPos[4];
static GLfloat SpotDir[3];
static GLfloat SpotAngle = 40.0 * DEG_TO_RAD;
static GLfloat ShadowNear = 4.0, ShadowFar = 24.0;
static GLint ShadowTexWidth = 512, ShadowTexHeight = 512;

static GLboolean LinearFilter = GL_FALSE;

static GLfloat Bias = -0.06;

//static GLboolean Anim = GL_TRUE;

//static GLint Operator = 0;
static const GLenum OperatorFunc[8] = {
   GL_LEQUAL, GL_LESS, GL_GEQUAL, GL_GREATER,
   GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS, GL_NEVER };
/*
static const char *OperatorName[8] = {
   "GL_LEQUAL", "GL_LESS", "GL_GEQUAL", "GL_GREATER",
   "GL_EQUAL", "GL_NOTEQUAL", "GL_ALWAYS", "GL_NEVER" };
*/

static GLuint DisplayMode = 0;
#define SHOW_NORMAL         0
#define SHOW_DEPTH_IMAGE    1
#define SHOW_DEPTH_MAPPING  2
#define SHOW_DISTANCE       3




/*
 * Load the GL_TEXTURE matrix with the projection from the light
 * source's point of view.
 */
static void
MakeShadowMatrix(const GLfloat lightPos[4], const GLfloat spotDir[3],
                 GLfloat spotAngle, GLfloat shadowNear, GLfloat shadowFar)
{
   GLfloat d;

   glMatrixMode(GL_TEXTURE);
   glLoadIdentity();
   glTranslatef(0.5, 0.5, 0.5 + Bias);
   glScalef(0.5, 0.5, 0.5);
   d = shadowNear * tan(spotAngle);
   glFrustum(-d, d, -d, d, shadowNear, shadowFar);
   gluLookAt(lightPos[0], lightPos[1], lightPos[2],
             lightPos[0] + spotDir[0],
             lightPos[1] + spotDir[1],
             lightPos[2] + spotDir[2],
             0, 1, 0);
   glMatrixMode(GL_MODELVIEW);
}


static void
EnableIdentityTexgen(void)
{
   /* texgen so that texcoord = vertex coord */
   static GLfloat sPlane[4] = { 1, 0, 0, 0 };
   static GLfloat tPlane[4] = { 0, 1, 0, 0 };
   static GLfloat rPlane[4] = { 0, 0, 1, 0 };
   static GLfloat qPlane[4] = { 0, 0, 0, 1 };

   glTexGenfv(GL_S, GL_EYE_PLANE, sPlane);
   glTexGenfv(GL_T, GL_EYE_PLANE, tPlane);
   glTexGenfv(GL_R, GL_EYE_PLANE, rPlane);
   glTexGenfv(GL_Q, GL_EYE_PLANE, qPlane);
   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

   glEnable(GL_TEXTURE_GEN_S);
   glEnable(GL_TEXTURE_GEN_T);
   glEnable(GL_TEXTURE_GEN_R);
   glEnable(GL_TEXTURE_GEN_Q);
}

/*
 * Setup 1-D texgen so that the distance from the light source, between
 * the near and far planes maps to s=0 and s=1.  When we draw the scene,
 * the grayness will indicate the fragment's distance from the light
 * source.
 */
static void
EnableDistanceTexgen(const GLfloat lightPos[4], const GLfloat lightDir[3],
                     GLfloat lightNear, GLfloat lightFar)
{
   GLfloat m, d;
   GLfloat sPlane[4];
   GLfloat nearPoint[3];

   m = sqrt(lightDir[0] * lightDir[0] +
            lightDir[1] * lightDir[1] +
            lightDir[2] * lightDir[2]);

   d = lightFar - lightNear;

   /* nearPoint = point on light direction vector which intersects the
    * near plane of the light frustum.
    */
   nearPoint[0] = LightPos[0] + lightDir[0] / m * lightNear;
   nearPoint[1] = LightPos[1] + lightDir[1] / m * lightNear;
   nearPoint[2] = LightPos[2] + lightDir[2] / m * lightNear;

   sPlane[0] = lightDir[0] / d / m;
   sPlane[1] = lightDir[1] / d / m;
   sPlane[2] = lightDir[2] / d / m;
   sPlane[3] = -(sPlane[0] * nearPoint[0]
               + sPlane[1] * nearPoint[1]
               + sPlane[2] * nearPoint[2]);

   glTexGenfv(GL_S, GL_EYE_PLANE, sPlane);
   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glEnable(GL_TEXTURE_GEN_S);
}

static void
DisableTexgen(void)
{
   glDisable(GL_TEXTURE_GEN_S);
   glDisable(GL_TEXTURE_GEN_T);
   glDisable(GL_TEXTURE_GEN_R);
   glDisable(GL_TEXTURE_GEN_Q);
}


static void
ComputeLightPos(GLfloat dist, GLfloat latitude, GLfloat longitude,
                GLfloat pos[4], GLfloat dir[3])
{

   pos[0] = dist * sin(longitude * DEG_TO_RAD);
   pos[1] = dist * sin(latitude * DEG_TO_RAD);
   pos[2] = dist * cos(latitude * DEG_TO_RAD) * cos(longitude * DEG_TO_RAD);
   pos[3] = 1;
   dir[0] = -pos[0];
   dir[1] = -pos[1];
   dir[2] = -pos[2];
}


static void
Display(ShadowMap::DrawFunc DrawScene)
{
   //GLfloat ar = (GLfloat) WindowWidth / (GLfloat) WindowHeight;
   GLfloat d;
   GLenum error;
   
std::cout << "Start...\n";
 
   AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
   AmjuGL::PushMatrix();
   AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
   AmjuGL::PushMatrix();
   AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);

   ComputeLightPos(LightDist, LightLatitude, LightLongitude,
                   LightPos, SpotDir);
   /*
    * Step 1: render scene from point of view of the light source
    */
   /* compute frustum to enclose spot light cone */
   d = ShadowNear * tan(SpotAngle);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-d, d, -d, d, ShadowNear, ShadowFar);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(LightPos[0], LightPos[1], LightPos[2], /* from */
             0, 0, 0, /* target */
             0, 1, 0); /* up */

   glViewport(0, 0, ShadowTexWidth, ShadowTexHeight);
   glClear(GL_DEPTH_BUFFER_BIT);

std::cout << "Draw from light POV...\n";
   DrawScene();
std::cout << "Done\n";

   /*
    * Step 2: copy depth buffer into texture map
    */
   if (DisplayMode == SHOW_DEPTH_MAPPING) {
      /* load depth image as gray-scale luminance texture */
      GLfloat *depth = (GLfloat *) malloc(ShadowTexWidth * ShadowTexHeight
                                          * sizeof(GLfloat));
      if (depth) {
         glReadPixels(0, 0, ShadowTexWidth, ShadowTexHeight,
                      GL_DEPTH_COMPONENT, GL_FLOAT, depth);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE,
                      ShadowTexWidth, ShadowTexHeight, 0,
                      GL_LUMINANCE, GL_FLOAT, depth);
         free(depth);
      }
   }
   else {
      /* The normal shadow case */
      glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                       0, 0, ShadowTexWidth, ShadowTexHeight, 0);
   }

   /*
    * Step 3: render scene from point of view of the camera
    */
   glViewport(0, 0, Screen::X(), Screen::Y()); ////WindowWidth, WindowHeight);
    
   AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
   AmjuGL::PopMatrix();
   AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
   AmjuGL::PopMatrix();

std::cout << "Stacks empty??\n";

   if (DisplayMode == SHOW_DEPTH_IMAGE) {
      ShowDepthBuffer(Screen::X(), Screen::Y(), 0, 1);
   }
   else 
   {
      /*
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(-ar, ar, -1.0, 1.0, 4.0, 50.0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glTranslatef(0.0, 0.0, -22.0);
      glRotatef(Xrot, 1, 0, 0);
      glRotatef(Yrot, 0, 1, 0);
      glRotatef(Zrot, 0, 0, 1);
      */

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
      if (LinearFilter) {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
      else {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }
      if (DisplayMode == SHOW_DEPTH_MAPPING) {
#if defined(GL_ARB_shadow)
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);
#elif defined(GL_SGIX_shadow)
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_SGIX, GL_FALSE);
#endif
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
         glEnable(GL_TEXTURE_2D);
         MakeShadowMatrix(LightPos, SpotDir, SpotAngle, ShadowNear, ShadowFar);
         EnableIdentityTexgen();
      }
      else if (DisplayMode == SHOW_DISTANCE) {
         glMatrixMode(GL_TEXTURE);
         glLoadIdentity();
         glMatrixMode(GL_MODELVIEW);
         EnableDistanceTexgen(LightPos, SpotDir, ShadowNear+Bias, ShadowFar);
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
         glEnable(GL_TEXTURE_1D);
      }
      else {
std::cout << "Setting up for 2nd pass...\n";
         Assert(DisplayMode == SHOW_NORMAL);
#if defined(GL_ARB_shadow)
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
                         GL_COMPARE_R_TO_TEXTURE_ARB);
#elif defined(GL_SGIX_shadow)
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_SGIX, GL_TRUE);
#endif
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
         glEnable(GL_TEXTURE_2D);
         MakeShadowMatrix(LightPos, SpotDir, SpotAngle, ShadowNear, ShadowFar);
         EnableIdentityTexgen();
      }

std::cout << "Drawing 2nd pass\n";
      DrawScene();
std::cout << "Done\n";

      DisableTexgen();
      glDisable(GL_TEXTURE_1D);
      glDisable(GL_TEXTURE_2D);
   }

//   glutSwapBuffers();
std::cout << "Finished!\n";

   error = glGetError();
   if (error) {
      printf("GL Error: %s\n", (char *) gluErrorString(error));
   }
}



void ShadowMapOpenGL1::Draw()
{
  static int i = 0;
  i++;

  if (i < 100)
  {
    m_drawFunc();
  }
  else if (i < 300)
  {
    Display(m_drawFunc);
    if (i < 200) 
    {
      DisplayMode = 0;
    }
    else
    {
      DisplayMode = 1;
    }
  }
  else if (i > 300)
  {
    i = 0;
  }

/*
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::PushMatrix();
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
    AmjuGL::PushMatrix();

    // Draw scene from light POV, copy z buffer (== shadow map)
    AmjuGL::SetIdentity();
    AmjuGL::SetPerspectiveProjection(90.0f, 1, 1, 1000); // TODO
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::SetIdentity();
    // TODO up vector
    AmjuGL::LookAt(m_lightPos.x, m_lightPos.y, m_lightPos.z,  0, 0, 0,  0, 1, 0);

    m_drawFunc();

    // Clear depth buf if necessary

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
    AmjuGL::PopMatrix();
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::PopMatrix();

    // Lighting etc
    AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);

    // Draw scene from camera POV. But use shadow algo to decide colour
    m_drawFunc();
*/
}
}


