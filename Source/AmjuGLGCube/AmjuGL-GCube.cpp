/*
Amju Games source code (c) Copyright Jason Colman 2009
*/

#include "AmjuFirst.h"

#include <iostream>
#include <stack>
#include <string.h> // for memset()
#include <malloc.h> // for memalign()
#include <math.h>
#include <gccore.h>
#include <gcmodplay.h>
#include <ogcsys.h>
#include <map>

#include "AmjuGL.h"
#include <ShaderNull.h>
#include "AmjuGL-GCube.h"
#include "TextureUtils.h"
#include "Pause.h" // TODO TEMP TEST
#include "Colour.h"
#include "Screen.h"
#include "AmjuFinal.h"

namespace Amju
{
// TODO Make all statics members

// Vars used for setting up screen and then for drawing
// These should only be used internally - so we can make them static.
static u32 curr_fb = 0;
static u32 first_frame = 1;
static void *xfb[2] = {NULL,NULL};
static GXRModeObj *rmode;

// Current modelview matrix 
Mtx s_modelviewMatrix; 

// Current projection matrix
Mtx44 s_projMatrix;
bool s_isOrtho = true; // true if projection is ortho, false if persp

// The three matrix stacks
// From gl2gx: mview matrix stack
static Mtx s_mtxelements[32]; // max mview stack depth is 32 in opengl
static int s_mtxcurrentelement = 0; // stack array index for mview

struct ProjStackElement
{
  Mtx44 mtx;
  bool isOrtho;
};

static ProjStackElement s_projStack[4]; // max size 4 ?
static int s_projCurrentElement = 0;

// Remember the current texture type. If sphere mapped, no need to send
// texture coords to the graphics card.
static AmjuGL::TextureType s_tt = AmjuGL::AMJU_TEXTURE_REGULAR;

// Store the current matrix mode
static int s_currentMatrix = 0;

// Current colour
Colour s_colour;

static bool s_lighting = false;
// Current lighting colours
Colour s_ambient;
Colour s_material;

//#define NO_LIGHTING

AmjuGLGCubeConsole::AmjuGLGCubeConsole()
{
}

void AmjuGLGCubeConsole::Init()
{
  // From console example - set up a console window for debugging/testing

  VIDEO_Init();
  
  rmode = VIDEO_GetPreferredMode(NULL);
  
  PAD_Init();
  
  xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    
  VIDEO_Configure(rmode);
    
  VIDEO_SetNextFramebuffer(xfb[0]);
  
  VIDEO_SetBlack(FALSE);
  VIDEO_Flush();
  VIDEO_WaitVSync();
  if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
    
  console_init(xfb[0],20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*2);
}

void AmjuGLGCubeConsole::Flip()
{
  // TODO I don't think we need this at all..?!
  VIDEO_WaitVSync(); // TODO does this just wait, or does it do something ?
  VIDEO_Flush();
}

AmjuGLGCube::AmjuGLGCube()
{
  m_texId = 0;

  static const int WIDTH = 640;
  static const int HEIGHT = 480; 
  Screen::SetSize(WIDTH, HEIGHT);
}

void AmjuGLGCube::BeginScene()
{
  AMJU_CALL_STACK;

  GX_InvVtxCache();

  // What does this do ? Do we really need it ?
  // ??? at the start of every scene ??
  //	GX_InvalidateTexAll(); 

  // Set Regular z test
  GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
  // to disable z test: GX_SetZMode (GX_FALSE, GX_NEVER, GX_TRUE);
  // TODO How disable z write ???
}

void AmjuGLGCube::EndScene()
{
}

void AmjuGLGCube::Flip()
{
  AMJU_CALL_STACK;
  
//  if (s_isConsole)
//  {
//    VIDEO_WaitVSync();
//    return;
//  }

  GX_SetColorUpdate(GX_TRUE);
  GX_CopyDisp(xfb[curr_fb],GX_TRUE);

  GX_DrawDone();

  VIDEO_SetNextFramebuffer(xfb[curr_fb]);

  if(first_frame) {
    first_frame = 0;
    VIDEO_SetBlack(FALSE);
  }	
  VIDEO_Flush();

  VIDEO_WaitVSync();
  curr_fb ^= 1;
}

// Taken from 	nehe lesson 8 port to GX by shagkur
void setlight(Mtx view,u32 theta,u32 phi,GXColor litcol, GXColor ambcol,GXColor matcol)
{
	guVector lpos;
	f32 _theta,_phi;
	GXLightObj lobj;

	_theta = (f32)theta*M_PI/180.0f;
	_phi = (f32)phi*M_PI/180.0f;
	lpos.x = 1000.0f * cosf(_phi) * sinf(_theta);
	lpos.y = 1000.0f * sinf(_phi);
	lpos.z = 1000.0f * cosf(_phi) * cosf(_theta);

	guVecMultiply(view,&lpos,&lpos);

	GX_InitLightPos(&lobj,lpos.x,lpos.y,lpos.z);

  GXColor myLightCol = { 0xff, 0xff, 0xff, 0xff };
	GX_InitLightColor(&lobj, myLightCol); //litcol);
	GX_LoadLightObj(&lobj,GX_LIGHT0);
	
	// set number of rasterized color channels
	//GX_SetNumChans(1);

  //j.c. in Draw code 
  // GX_SetChanCtrl(GX_COLOR0A0,GX_ENABLE,GX_SRC_REG,GX_SRC_REG,GX_LIGHT0,GX_DF_CLAMP,GX_AF_NONE);

  //s_ambient.Set(ambcol.r, ambcol.g, ambcol.b, ambcol.a);
  //s_material.Set(matcol.r, matcol.g, matcol.b, matcol.a);
}

void AmjuGLGCube::DrawLighting(
  const AmjuGL::LightColour& globalAmbient, 
  const AmjuGL::LightColour& lightAmbient,
  const AmjuGL::LightColour& lightDiffuse,
  const AmjuGL::LightColour& lightSpecular,
  const AmjuGL::Vec3& lightPos)
{
  AMJU_CALL_STACK;

#ifndef NO_LIGHTING
  /*
  // Example from nehe #8 sample
  static GXColor litcolors[] = {
        { 0xD0, 0xD0, 0xD0, 0xFF }, // Light color 1
        { 0x40, 0x40, 0x40, 0xFF }, // Ambient 1
        { 0x80, 0x80, 0x80, 0xFF }  // Material 1
  };

  // Angle of light ?, degs 
  float theta = 0;
  float phi = 45.0f;
  setlight(s_modelviewMatrix, theta, phi, litcolors[0],litcolors[1],litcolors[2]);
  */

	guVector lpos;
	GXLightObj lobj;

	lpos.x = lightPos.m_x; 
	lpos.y = -lightPos.m_y; // looks like this is reversed - or is it all three ?? 
	lpos.z = lightPos.m_z; 

  // No good
	//guVecMultiply(s_modelviewMatrix,&lpos,&lpos);

  // This + the normal matrix seems to do the job..?
	GX_InitLightPos(&lobj,lpos.x * 1000.0f, lpos.y * 1000.0f, lpos.z * 1000.0f); // ? -Y ?
//  GX_InitLightDir(&lobj, lpos.x, lpos.y, lpos.z); 
  // TODO Both needed ???

  GXColor myLightCol = { 0xff, 0xff, 0xff, 0xff };
	GX_InitLightColor(&lobj, myLightCol); //litcol);
	GX_LoadLightObj(&lobj,GX_LIGHT0);
	
  s_ambient.Set(0.5f, 0.5f, 0.5f, 1.0f); //ambcol.r, ambcol.g, ambcol.b, ambcol.a);
//  s_ambient.Set(lightAmbient.m_r, lightAmbient.m_g, lightAmbient.m_b, 1.0f);
  s_material.Set(1, 1, 1, 1); //matcol.r, matcol.g, matcol.b, matcol.a);

#endif // NO_LIGHTING
}

bool AmjuGLGCube::CreateWindow(AmjuGLWindowInfo*)
{
  return true;
}

#define DEFAULT_FIFO_SIZE	(256*1024)		//GX_FIFO_MINSIZE

void AmjuGLGCube::Init()
{
  // See explanation at http://gxr.brickmii.com/Tutorial/Setup/

  AMJU_CALL_STACK;
  
  f32 yscale;
  u32 xfbHeight;
 
  // init the vi. setup frame buffer and set the retrace callback
  // to copy the efb to xfb
  VIDEO_Init();
  PAD_Init();
 
  rmode = VIDEO_GetPreferredMode(NULL);
 
 
  curr_fb = 0;
  first_frame = 0;
 
  // setup the fifo and then init the flipper
  void *gp_fifo = NULL;
  gp_fifo = memalign(32,DEFAULT_FIFO_SIZE);
  memset(gp_fifo,0,DEFAULT_FIFO_SIZE);
 
  xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
  xfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

  VIDEO_Configure(rmode);
  VIDEO_SetNextFramebuffer(xfb[curr_fb]);
  if(!first_frame) VIDEO_SetBlack(FALSE);
  VIDEO_Flush();
  VIDEO_WaitVSync();
  if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
  console_init(xfb[curr_fb],20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
 
  //curr_fb ^= 1;
 
  GX_Init(gp_fifo,DEFAULT_FIFO_SIZE);
 
  // clears the bg to color and clears the z buffer
  GXColor background = {0, 0, 0, 0xff};
  GX_SetCopyClear(background, 0x00ffffff);
 
  // other gx setup
  GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
  yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
  xfbHeight = GX_SetDispCopyYScale(yscale);
  GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
  GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
  GX_SetDispCopyDst(rmode->fbWidth,xfbHeight);
  GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
  GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));
 
  GX_SetCullMode(GX_CULL_FRONT); // j.c. hmm, well it seems to work. 
  GX_CopyDisp(xfb[curr_fb],GX_TRUE);
  GX_SetDispCopyGamma(GX_GM_1_0);
}

void AmjuGLGCube::InitFrame(float clearR, float clearG, float clearB)
{
  AMJU_CALL_STACK;

  GXColor background = { clearR * 0xff, clearG * 0xff, clearB * 0xff, 0xff};
  GX_SetCopyClear(background, 0x00ffffff);

  // TODO Enable/disable blending
  //GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
}

void AmjuGLGCube::SetPerspectiveProjection(
  float fov, float aspectRatio, float nearDist, float farDist)
{
  AMJU_CALL_STACK;

  guPerspective(s_projMatrix, fov, aspectRatio, nearDist, farDist);
  GX_LoadProjectionMtx(s_projMatrix, GX_PERSPECTIVE); 
  s_isOrtho = false;
}

void AmjuGLGCube::Viewport(int x, int y, int w, int h)
{
  AMJU_CALL_STACK;

  // Need to do both of these to get the expected effect
  GX_SetViewport(x, y, w, h, 0, 1);
  GX_SetScissor(x, y, w, h);
}

void AmjuGLGCube::LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ)
{
  AMJU_CALL_STACK;

  // setup our camera at the origin
  // looking down the -z axis with y up
  guVector cam = { eyeX, eyeY, eyeZ },  //{0.0F, 0.0F, 0.0F},
      up = { upX, upY, upZ },     //{0.0F, 1.0F, 0.0F},
      look = { x, y, z };           //{0.0F, 0.0F, -1.0F};
  guLookAt(s_modelviewMatrix, &cam, &up, &look);
}

void AmjuGLGCube::SetColour(float r, float g, float b, float a)
{
  AMJU_CALL_STACK;

  s_colour = Colour(r, g, b, a);
}

void PrepareToDraw()
{
  AMJU_CALL_STACK;

  // Load modelview matrix
  GX_LoadPosMtxImm(s_modelviewMatrix, GX_PNMTX0);

  // Required for lighting..??
  
  Mtx mvi, mvit;
	guMtxInverse(s_modelviewMatrix, mvi);
	guMtxTranspose(mvi, mvit);
	GX_LoadNrmMtxImm(mvit, GX_PNMTX0);
  
  // A lot of this is the same for all drawing, so maybe we can do it once up front ???

  // Set vertex description, i.e. how we will specify verts..?
  // Direct as opposed to indexed..?
  
  GX_ClearVtxDesc();
  GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
  
  // Per-vertex Colour: Maybe use colour set in SetColour above
  GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);

  // Tex coord format: not indexed
  GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
  // GX_VA_TEX0  -- NB looks like you can multitexture up to 8 textures ??
  
  // Normals
  GX_SetVtxDesc(GX_VA_NRM, GX_DIRECT);

  // setup the vertex attribute table
  // describes the data
  // args: vat location 0-7, type of data, data format, size, scale
  // so for ex. in the first call we are sending position data with
  // 3 values X,Y,Z of size F32. scale sets the number of fractional
  // bits for non float data.
  GX_SetVtxAttrFmt(
    GX_VTXFMT0, // VAT (vertex attribute table) location, 0-7
    GX_VA_POS,  // type of data: positional
    GX_POS_XYZ, // format
    GX_F32,     // size: 32-bit floats
    0);         // scale: what is this for ? TODO
    
  // Set colour format 
  GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

  // Set tex coord format
  GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

  // Normals format
  GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
 
#ifndef NO_LIGHTING
  if (s_lighting)
  {
    GX_SetChanCtrl(
      GX_COLOR0A0,GX_ENABLE,GX_SRC_REG,GX_SRC_REG,GX_LIGHT0,GX_DF_CLAMP,GX_AF_NONE);

    Colour amb = s_ambient; // * s_colour;
    Colour mat = s_ambient * s_colour; // only one needs to be multiplied ???

    GXColor a = { amb.m_r * 255.0f, amb.m_g * 255.0f, amb.m_b * 255.0f, amb.m_a * 255.0f };
    GXColor m = { mat.m_r * 255.0f, mat.m_g * 255.0f, mat.m_b * 255.0f, mat.m_a * 255.0f };

    GX_SetChanAmbColor(GX_COLOR0A0, a);
    GX_SetChanMatColor(GX_COLOR0A0, m);
  }
  else
  {
    // TODO This breaks TRANSPARENCY, i.e. alpha != 255
    GX_SetChanCtrl(
      GX_COLOR0A0,GX_DISABLE,GX_SRC_REG,GX_SRC_REG,GX_LIGHT0,GX_DF_CLAMP,GX_AF_NONE);

    GXColor col = 
    { 
      s_colour.m_r * 255.0f, 
      s_colour.m_g * 255.0f, 
      s_colour.m_b * 255.0f, 
      s_colour.m_a * 255.0f 
    };
    GX_SetChanAmbColor(GX_COLOR0A0, col);
    GX_SetChanMatColor(GX_COLOR0A0, col);
  }
#endif // NO_LIGHTING

  // Set up texture format
  GX_SetNumChans(1);
  GX_SetNumTexGens(1);
  
  // setup texture coordinate generation
  // args: texcoord slot 0-7, matrix type, source to generate texture coordinates from, matrix to use  
  GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
  
  ////GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
  GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

  // TODO TUrn off blending if not required
  GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
  
  // need to set colours at verts so we have a colour to modulate 
  GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE); 
  
//	GX_SetTevOp(GX_TEVSTAGE0, GX_BLEND); //= no textures, just white (no lighting)
//  GX_SetTevOp(GX_TEVSTAGE0, GX_DECAL); // no blending! :-( 
 
}

void AmjuGLGCube::DrawTriList(const AmjuGL::Tris& tris)
{
  AMJU_CALL_STACK;

  PrepareToDraw();

  u8 r = (u8)(s_colour.m_r * 255.0);
  u8 g = (u8)(s_colour.m_g * 255.0);
  u8 b = (u8)(s_colour.m_b * 255.0);
  u8 a = (u8)(s_colour.m_a * 255.0);

  // Start drawing triangles
  unsigned int numTris = tris.size();
  GX_Begin(GX_TRIANGLES, GX_VTXFMT0, numTris * 3);		
  
    for (unsigned int i = 0; i < numTris; i++)
    {
      const AmjuGL::Tri& tri = tris[i]; 
      for (int j = 0; j < 3; j++)
      {			
        const AmjuGL::Vert& v = tri.m_verts[j];
        
        // Pos comes first, unlike OpenGL..?
        GX_Position3f32(v.m_x, v.m_y, v.m_z);		
        GX_Normal3f32(v.m_nx, v.m_ny, v.m_nz);
        GX_Color4u8(r, g, b, a); // TODO Do we have to specify this for every vertex ?
        GX_TexCoord2f32(v.m_u, v.m_v);
      }
    }
  
  GX_End();
}

void AmjuGLGCube::DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2)
{
  AMJU_CALL_STACK;

  PrepareToDraw();

  GX_SetTevOp(GX_TEVSTAGE0, GX_BLEND); //= no textures, just white 
  // (will this still work with lighting ?)

  u8 r = (u8)(s_colour.m_r * 255.0);
  u8 g = (u8)(s_colour.m_g * 255.0);
  u8 b = (u8)(s_colour.m_b * 255.0);
  u8 a = (u8)(s_colour.m_a * 255.0);
  
  // Start drawing lines
  GX_Begin(GX_LINES, GX_VTXFMT0, 2);		
  
    // Pos comes first, unlike OpenGL..?
    GX_Position3f32(v1.m_x, v1.m_y, v1.m_z);		
    GX_Normal3f32(1, 0, 0); // Need to specify a normal
    GX_Color4u8(r, g, b, a); // TODO Do we have to specify this for every vertex ?
    GX_TexCoord2f32(0, 0);//v.m_u, v.m_v);

    GX_Position3f32(v2.m_x, v2.m_y, v2.m_z);		
    GX_Normal3f32(1, 0, 0);
    GX_Color4u8(r, g, b, a); // TODO Do we have to specify this for every vertex ?
    GX_TexCoord2f32(0, 0);//v.m_u, v.m_v);
  
  GX_End();
}

void AmjuGLGCube::DrawQuadList(const AmjuGL::Quads& quads)
{
  AMJU_CALL_STACK;


  // Can use GX_QUADS
}

void AmjuGLGCube::DrawQuad(AmjuGL::Vert* verts)
{
  AMJU_CALL_STACK;

  // Or Can use GX_QUADS

  AmjuGL::Tris tris;
	tris.reserve(2);
	AmjuGL::Tri tri;
	tri.m_verts[0] = verts[0];
	tri.m_verts[1] = verts[1];
	tri.m_verts[2] = verts[2];
	tris.push_back(tri);

	tri.m_verts[0] = verts[0];
	tri.m_verts[1] = verts[2];
	tri.m_verts[2] = verts[3];
	tris.push_back(tri);

  DrawTriList(tris);
}

void AmjuGLGCube::DrawIndexedTriList(
  const AmjuGL::Verts& verts,
  const AmjuGL::IndexedTriList& indexes)
{
  AMJU_CALL_STACK;

  // TODO
}

void AmjuGLGCube::GetMatrix(AmjuGL::MatrixMode m, float result[16])
{
  AMJU_CALL_STACK;
  
  switch (m)
  {
  case AmjuGL::AMJU_MODELVIEW_MATRIX:
    // This should be equivalent to glGetFloatv(GL_MODELVIEW_MATRIX, result);
    result[0] = s_modelviewMatrix[0][0];
    result[1] = s_modelviewMatrix[1][0];
    result[2] = s_modelviewMatrix[2][0];
    result[3] = 0;

    result[4] = s_modelviewMatrix[0][1];
    result[5] = s_modelviewMatrix[1][1];
    result[6] = s_modelviewMatrix[2][1];
    result[7] = 0;

    result[8] = s_modelviewMatrix[0][2];
    result[9] = s_modelviewMatrix[1][2];
    result[10]= s_modelviewMatrix[2][2];
    result[11]= 0;

    result[12]= s_modelviewMatrix[0][3];    
    result[13]= s_modelviewMatrix[1][3];
    result[14]= s_modelviewMatrix[2][3];
    result[15]= 1.0f;

    return;

  case AmjuGL::AMJU_PROJECTION_MATRIX:
    // This should be equivalent to glGetFloatv(GL_PROJECTION_MATRIX, result);

    // memcpy(dest, src, size)
    memcpy(result, s_projMatrix, 16*sizeof(float));
    return;

  case AmjuGL::AMJU_TEXTURE_MATRIX:
    // TODO 
    //glGetFloatv(GL_TEXTURE_MATRIX, result);
    Assert(0);
    return;

  default:
    Assert(0);
  }
}

void AmjuGLGCube::MultMatrix(const float matrix[16])
{
  Assert(s_currentMatrix == 0); // only works with modelview matrix
  
  Mtx m;
  // typedef f32	Mtx[3][4];
  m[0][0] = matrix[0];
  m[1][0] = matrix[1];
  m[2][0] = matrix[2];

  m[0][1] = matrix[4];
  m[1][1] = matrix[5];
  m[2][1] = matrix[6];

  m[0][2] = matrix[8];
  m[1][2] = matrix[9];
  m[2][2] = matrix[10];

  m[0][3] = matrix[12];
  m[1][3] = matrix[13];
  m[2][3] = matrix[14];

  guMtxConcat(s_modelviewMatrix, m, s_modelviewMatrix);
}

void AmjuGLGCube::SetMatrixMode(AmjuGL::MatrixMode m)
{
  AMJU_CALL_STACK;

  switch (m)
  {
  case AmjuGL::AMJU_MODELVIEW_MATRIX:
    s_currentMatrix = 0;
    return;    

  case AmjuGL::AMJU_PROJECTION_MATRIX:
    s_currentMatrix = 1;
    return;

  case AmjuGL::AMJU_TEXTURE_MATRIX:
    s_currentMatrix = 2;
    return;

  default:
    Assert(0);
    break;
  }
}

void AmjuGLGCube::SetIdentity()
{
  AMJU_CALL_STACK;

  switch (s_currentMatrix)
  {
  case 0:
    guMtxIdentity(s_modelviewMatrix);
    break;
  case 1:
    {
      // For orthographic projection (2D HUD etc)
      // It seems that y = -1 is the top of the viewport, and y = 1 is the bottom ?
      guOrtho(s_projMatrix,  1, -1, -1, 1, -1, 1); // t, b, l, r, n, f
      // Not: guOrtho(s_projMatrix, -1, 1, -1, 1, -1, 1); 
      s_isOrtho = true;
      GX_LoadProjectionMtx(s_projMatrix, GX_ORTHOGRAPHIC);  
    }
    break;
  default:
    Assert(0);
  }
}

void AmjuGLGCube::PushMatrix()
{
  AMJU_CALL_STACK;

  switch (s_currentMatrix)
  {
  case 0:
    // guMtxCopy(src, dest) 
    guMtxCopy(s_modelviewMatrix, s_mtxelements[s_mtxcurrentelement]);
    s_mtxcurrentelement++;
    break;
  case 1:
    // memcpy(dest, src, size)
    memcpy(s_projStack[s_projCurrentElement].mtx, s_projMatrix, 16*sizeof(float));
    s_projStack[s_projCurrentElement].isOrtho = s_isOrtho;
    s_projCurrentElement++;
    break;
  default:
    Assert(0);
  }
  
/* gl2gx code:
  guMtxCopy(model, _mtxelements[_mtxcurrentelement]);
  _mtxcurrentelement++;
*/
}

void AmjuGLGCube::PopMatrix()
{
  AMJU_CALL_STACK;

  switch (s_currentMatrix)
  {
  case 0: 
    s_mtxcurrentelement--;
    // guMtxCopy(src, dest) 
    guMtxCopy(s_mtxelements[s_mtxcurrentelement], s_modelviewMatrix);
    break;
  case 1:
    s_projCurrentElement--;
    // memcpy(dest, src, size)
    memcpy(s_projMatrix, s_projStack[s_projCurrentElement].mtx, 16*sizeof(float));
    s_isOrtho = s_projStack[s_projCurrentElement].isOrtho;
    // Now set new proj matrix
    GX_LoadProjectionMtx(s_projMatrix, s_isOrtho ? GX_ORTHOGRAPHIC : GX_PERSPECTIVE); 
    break;
  default:
    Assert(0);
  }
}

void AmjuGLGCube::Translate(float x, float y, float z)
{
  AMJU_CALL_STACK;

  Mtx temp;
  guMtxIdentity(temp);
  guMtxTrans(temp, x, y, z);	
  guMtxConcat(s_modelviewMatrix,temp,s_modelviewMatrix);
}

void AmjuGLGCube::Scale(float x, float y, float z)
{
  AMJU_CALL_STACK;

  Mtx temp;
  guMtxIdentity(temp);
  guMtxScale(temp, x, y, z);
  guMtxConcat(s_modelviewMatrix,temp,s_modelviewMatrix);
}

void AmjuGLGCube::RotateX(float degs)
{
  AMJU_CALL_STACK;

  guVector axis = {1, 0, 0};
  
  Mtx m; // model matrix.
  guMtxIdentity(m);
  guMtxRotAxisDeg(m, &axis, degs);
  guMtxConcat(s_modelviewMatrix,m,s_modelviewMatrix);
}

void AmjuGLGCube::RotateY(float degs)
{
  AMJU_CALL_STACK;

  guVector axis = {0, 1, 0};
  Mtx m; // model matrix.
  guMtxIdentity(m);
  guMtxRotAxisDeg(m, &axis, degs);
  guMtxConcat(s_modelviewMatrix,m,s_modelviewMatrix);
}

void AmjuGLGCube::RotateZ(float degs)
{
  AMJU_CALL_STACK;

  guVector axis = {0, 0, 1};
  Mtx m; // model matrix.
  guMtxIdentity(m);
  guMtxRotAxisDeg(m, &axis, degs);
  guMtxConcat(s_modelviewMatrix,m,s_modelviewMatrix);
}

void AmjuGLGCube::PushAttrib(uint32 attrib)
{
  AMJU_CALL_STACK;

  // Push/Pop Attrib is emulated in AmjuGL -- Enable/Disable is called, 
  //  no need to use glPushAttrib/glPopAttrib.
  // This is for consistency and also is good for OpenGL ES.
}

void AmjuGLGCube::PopAttrib()
{
  AMJU_CALL_STACK;
}

void AmjuGLGCube::Enable(uint32 flag)
{
  AMJU_CALL_STACK;

  if (flag & AmjuGL::AMJU_DEPTH_READ)
  {
    // Enable Z test  
    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
  }
  if (flag & AmjuGL::AMJU_LIGHTING)
  {
    s_lighting = true;
  }
}

void AmjuGLGCube::Disable(uint32 flag)
{
  AMJU_CALL_STACK;

  if (flag & AmjuGL::AMJU_DEPTH_READ)
  {
    // Disable Z test... seems to do what we want..  
    GX_SetZMode (GX_FALSE, GX_NEVER, GX_TRUE); 
  }

  if (flag & AmjuGL::AMJU_LIGHTING)
  {
    s_lighting = false;
  }
}

void AmjuGLGCube::BlendFunc()
{
  AMJU_CALL_STACK;
}

void AmjuGLGCube::EnableZWrite(bool b)
{
  AMJU_CALL_STACK;
}

void AmjuGLGCube::DestroyTextureHandle(AmjuGL::TextureHandle* th)
{
  AMJU_CALL_STACK;
  
  int texId = (int)(*th);

//#ifdef _DEBUG
  if (m_textures.find(texId) == m_textures.end())
  {
    EndScene(); // TODO TEMP TEST
    std::cout << "\n*** Texture handle not found: " << texId << "\n";
    std::cout << "Currently there are " << m_textures.size() << " in the map...\n";

    Assert(0);
  }
//#endif

  TexData& td = m_textures[texId];
  unsigned char* data = td.m_data;
  free(data); // not delete; allocated with memalign
  GXTexObj* g = td.m_texObj;
  delete g;
  m_textures.erase(texId);
}

void AmjuGLGCube::SetTextureMode(AmjuGL::TextureType tt)
{
  AMJU_CALL_STACK;

  s_tt = tt;
}


/**
 * Convert a raw BMP (RGB, no alpha) to 4x4RGBA.
 * @author DragonMinded
 * @param src
 * @param dst
 * @param width
 * @param height
*/
static void Raw24To4x4RGBA(
  const unsigned char *src, void *dst, const unsigned int width, const unsigned int height) 
{
    unsigned int block;
    unsigned int i;
    unsigned int c;
    unsigned int ar;
    unsigned int gb;
    unsigned char *p = (unsigned char*)dst;

    for (block = 0; block < height; block += 4) {
        for (i = 0; i < width; i += 4) {
            /* Alpha and Red */
            for (c = 0; c < 4; ++c) {
                for (ar = 0; ar < 4; ++ar) {
                    /* Alpha pixels */
                    *p++ = 255;
                    /* Red pixels */
                    *p++ = src[((i + ar) + ((block + c) * width)) * 3];
                }
            }

            /* Green and Blue */
            for (c = 0; c < 4; ++c) {
                for (gb = 0; gb < 4; ++gb) {
                    /* Green pixels */
                    *p++ = src[(((i + gb) + ((block + c) * width)) * 3) + 1];
                    /* Blue pixels */
                    *p++ = src[(((i + gb) + ((block + c) * width)) * 3) + 2];
                }
            }
        } /* i */
    } /* block */
}

// Convert RGBA image to 4x4 RGBA format
// From http://www.plaatsoft.nl/wiibrew/2009/03/
static void Raw32To4x4RGBA(
  const unsigned char *src, 
  void *dst,
  const unsigned int width, 
  const unsigned int height)
{
  unsigned int block = 0;
  unsigned int i = 0;
  unsigned int c = 0;
  unsigned int ar = 0;
  unsigned int gb = 0;
  unsigned char *p = (unsigned char*)dst;

  for (block = 0; block < height; block += 4) 
  {
    for (i = 0; i < width; i += 4) 
    {
      /* Alpha and Red */
      for (c = 0; c < 4; ++c) 
      {
        for (ar = 0; ar < 4; ++ar) 
        {
          /* Alpha pixels */
          *p++ = src[(((i + ar) + ((block + c) * width)) * 4) + 3];
          /* Red pixels */
          *p++ = src[((i + ar) + ((block + c) * width)) * 4];
        }
      }

      /* Green and Blue */
      for (c = 0; c < 4; ++c) 
      {
        for (gb = 0; gb < 4; ++gb) 
        {
          /* Green pixels */
          *p++ = src[(((i + gb) + ((block + c) * width)) * 4) + 1];
          /* Blue pixels */
          *p++ = src[(((i + gb) + ((block + c) * width)) * 4) + 2];
        }
      }
    } /* i */
  } /* block */
}


// Forum discussion about texture data:
//  http://forum.wiibrew.org/read.php?11,11948



void AmjuGLGCube::SetTexture(
  AmjuGL::TextureHandle* th, 
  AmjuGL::TextureType tt, 
  AmjuGL::TextureDepth d, 
  int width, 
  int height, 
  uint8* data)
{
  AMJU_CALL_STACK;

  GXTexObj* g = new GXTexObj;  

  // Convert to correct format
  unsigned char* convertedData = 
    (unsigned char*)memalign(32, width*height*4);
    // NOT: new unsigned char[width * height * 4];
    //  -- must be aigned to 32byte boundary
      
  if (d == AmjuGL::AMJU_RGB)
  {
    // Convert to strange format
    Raw24To4x4RGBA(data, convertedData, width, height);          
  }
  else // d == AmjuGL::RGBA
  {
    // Convert to strange format
    Raw32To4x4RGBA(data, convertedData, width, height);
  }

  GX_InitTexObj(g, convertedData, width, height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE); 
  
  // This is needed - what does it do ?
  DCFlushRange(convertedData, width * height * 4);

  m_textures[m_texId] = TexData(g, convertedData);
  *th = m_texId;
  ++m_texId;
}

void AmjuGLGCube::UseTexture(AmjuGL::TextureHandle th)
{
  AMJU_CALL_STACK;

  int texId = (int)th;
  Assert(m_textures.find(texId) != m_textures.end());
  GXTexObj* g = m_textures[texId].m_texObj;
  GX_LoadTexObj(g, GX_TEXMAP0);  
}

void AmjuGLGCube::GetScreenshot(unsigned char* buffer, int w, int h)
{
  AMJU_CALL_STACK;
}

Shader* AmjuGLGCube::LoadShader(const std::string& )
{
  return new ShaderNull;
}
}


