#ifndef IPHONE

#include <iostream>
#include "ShadowMapOpenGL2.h"
#include "OpenGL.h"
#include <stdlib.h>
#include "GLShader.h"
#include <math.h>
#include <Screen.h>

// This implementation is adapted from http://fabiensanglard.net/shadowmapping/
// TODO: Add lighting, get this to work with textures

// Shows shadow map but Breaks textures :-(
//#define SHOW_MAP

namespace Amju
{
GLShader* shader;


void loadShader()
{
  shader = new GLShader;

  shader->Create(
    // Vertex shader
    "varying vec4 ShadowCoord; \n"
    "void main() \n"
    "{ \n"
    "    ShadowCoord = gl_TextureMatrix[7] * gl_Vertex; \n" 
    "    gl_Position = ftransform(); "
    "    vec3 normal = normalize(gl_NormalMatrix * gl_Normal); \n"
    "    vec3 lightDir = normalize(vec3(gl_LightSource[0].position)); \n"
    "    float NdotL = max(dot(normal, lightDir), 0.0); \n"
    "    vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse; \n"
    "    vec4 AMBIENT = vec4(0.3, 0.3, 0.3, 1); \n"
    "    gl_FrontColor = gl_Color * NdotL * diffuse + AMBIENT; \n"
    "} ",

    // Fragment shader
    "uniform sampler2D ShadowMap; \n"
    "varying vec4 ShadowCoord; \n"
    "void main() \n"
    "{ \n"
    "    vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ; \n"
    "    // Used to lower moiré pattern and self-shadowing \n"
    "    shadowCoordinateWdivide.z += 0.0005; \n"
    "    float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.st).z; \n"
    "    float shadow = 1.0; \n"
    "    if (ShadowCoord.w > 0.0) \n"
    "            shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.2 : 1.0 ; \n"
    "    gl_FragColor = shadow * gl_Color; \n"
    "} ");
}

#define SHADOW_MAP_RATIO 4 

// Hold id of the framebuffer for light POV rendering
GLuint fboId;

// Z values will be rendered to this texture when using fboId framebuffer
GLuint depthTextureId;

void generateShadowFBO()
{
  GLenum FBOstatus;

  // Try to use a texture depth component
  glGenTextures(1, &depthTextureId);
  glBindTexture(GL_TEXTURE_2D, depthTextureId);

  // GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Remove artefact on the edges of the shadowmap
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

  //glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );

  int shadowMapWidth = Screen::X() * SHADOW_MAP_RATIO;
  int shadowMapHeight = Screen::Y() * SHADOW_MAP_RATIO;


  // No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
  glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  // create a framebuffer object
  glGenFramebuffersEXT(1, &fboId);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

  // Instruct openGL that we won't bind a color texture with the currently binded FBO
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  // attach the texture to FBO depth attachment point
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, depthTextureId, 0);

  // check FBO status
  FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
    printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");

  std::cout << "Created FBO ok\n";

  // switch back to window-system-provided framebuffer
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void setupMatrices(float position_x,float position_y,float position_z,float lookAt_x,float lookAt_y,float lookAt_z)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, 1.0, 10, 40000); //0.1, 1000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);
}

void setTextureMatrix(void)
{
  static float modelView[16];
  static float projection[16];

  // This is matrix transform every coordinate x,y,z
  // x = x* 0.5 + 0.5
  // y = y* 0.5 + 0.5
  // z = z* 0.5 + 0.5
  // Moving from unit cube [-1,1] to [0,1]
  const GLfloat bias[16] = 
  {
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
  };

  // Grab modelview and transformation matrices
  glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
  glGetFloatv(GL_PROJECTION_MATRIX, projection);


  glMatrixMode(GL_TEXTURE);
  glActiveTextureARB(GL_TEXTURE7);

  glLoadIdentity();
  glLoadMatrixf(bias);

  // concatating all matrice into one.
  glMultMatrixf(projection);
  glMultMatrixf(modelView);

  // Go back to normal matrix mode
  glMatrixMode(GL_MODELVIEW);
}

void ShadowMapOpenGL2::Draw()
{
  // j.c. we also need this
  glBindTexture(GL_TEXTURE_2D,depthTextureId);

  //First step: Render from the light POV to a FBO, story depth values only
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId); //Rendering offscreen

  //Using the fixed pipeline to render to the depthbuffer
  glUseProgramObjectARB(0);

  int shadowMapWidth = Screen::X() * SHADOW_MAP_RATIO;
  int shadowMapHeight = Screen::Y() * SHADOW_MAP_RATIO;

  // In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
  glViewport(0,0, shadowMapWidth, shadowMapHeight) ;

  // Clear previous frame values
  glClear( GL_DEPTH_BUFFER_BIT);

  //Disable color rendering, we only want to write to the Z-Buffer
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  // j.c. store old matrices
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  //Light position
  float p_light[4] = { m_lightPos.m_x, m_lightPos.m_y, m_lightPos.m_z, 0 };
  glLightfv(GL_LIGHT0, GL_POSITION, p_light);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  //Light lookAt
  float l_light[3] = {0,0,0};

  setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);

  // Culling switching, rendering only backface, this is done to avoid self-shadowing
  glCullFace(GL_FRONT);
  m_drawFunc();

  //Save modelview/projection matrice into texture7, also add a bias
  setTextureMatrix();


  // Now rendering from the camera POV, using the FBO to generate shadows
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);

  glViewport(0,0, Screen::X(), Screen::Y()); //RENDER_WIDTH,RENDER_HEIGHT);

  //Enabling color write (previously disabled for light POV z-buffer rendering)
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  // Clear previous frame values
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Using the shadow shader
  Assert(shader);
  shader->Begin();

  shader->Set(std::string("ShadowMap"), (unsigned int)7);

  glActiveTextureARB(GL_TEXTURE7);
  glBindTexture(GL_TEXTURE_2D,depthTextureId);

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  glCullFace(GL_BACK);
  m_drawFunc();
  shader->End();

  // DEBUG only. this piece of code draw the depth buffer onscreen
#ifdef SHOW_MAP
  glUseProgramObjectARB(0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-Screen::X()/2, Screen::X()/2, -Screen::Y()/2, Screen::Y()/2,1,20);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor4f(1,1,1,1);
  glActiveTextureARB(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,depthTextureId);
  glEnable(GL_TEXTURE_2D);
  glTranslated(0,0,-1);
  glBegin(GL_QUADS);
  glTexCoord2d(0,0);glVertex3f(0,0,0);
  glTexCoord2d(1,0);glVertex3f(Screen::X()/2,0,0);
  glTexCoord2d(1,1);glVertex3f(Screen::X()/2,Screen::Y()/2,0);
  glTexCoord2d(0,1);glVertex3f(0,Screen::Y()/2,0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
#endif

  // Force textures to re-bind
  glActiveTextureARB(GL_TEXTURE0);
  AmjuGL::UseTexture(0);
}


bool ShadowMapOpenGL2::Init()
{
  generateShadowFBO();

  loadShader();

  // This is important, if not here, FBO's depthbuffer won't be populated.
  glEnable(GL_DEPTH_TEST);
  glClearColor(0,0,0,1.0f);

  glEnable(GL_CULL_FACE);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

  return true;
}

}

#endif 

