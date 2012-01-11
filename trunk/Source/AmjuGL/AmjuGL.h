/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#ifndef AMJU_GL_H_INCLUDED
#define AMJU_GL_H_INCLUDED

#include <ostream>
#include <string>
#include <vector>
#include <AmjuTypes.h>
#include "Colour.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace Amju
{
class AmjuGLImpl;
class AmjuGLWindowInfo;
class Shader;
class ShadowMap;

struct AmjuGL
{
public:
  static void SetImpl(AmjuGLImpl*);

  static std::ostream& ReportState(std::ostream&);

  enum Flags
  {
    AMJU_DEPTH_READ  = 0x0001,
    AMJU_LIGHTING    = 0x0002,
    AMJU_BLEND       = 0x0004,
    AMJU_TEXTURE_2D  = 0x0008,
    AMJU_DEPTH_WRITE = 0x0010
  };

  // Vertex: (x, y, z) abs coord, (u, v) texture coord and normal.
  struct Vert
  {
    Vert() {}
    Vert(float x, float y, float z, float u, float v, float nx, float ny, float nz);

    // Order for D3D FVF
    float m_x, m_y, m_z, m_nx, m_ny, m_nz, m_u, m_v;

    // We need this format for GL interleaved arrays, i.e. GL_T2F_N3F_V3F
    //float m_u, m_v,  m_nx, m_ny, m_nz,  m_x, m_y, m_z;
  };
  typedef std::vector<Vert> Verts;

  struct Vec3
  {
    Vec3() {}
    Vec3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}
    float m_x, m_y, m_z;
  };

  // For non-indexed triangle list, this is simply the 3 verts of a triangle.
  struct Tri
  {
    Vert m_verts[3];
  };

  typedef std::vector<Tri> Tris;

  struct IndexedTriList
  { 
    // List of tris. Each tri has 3 verts, 3 UVs and 3 normals (x, y, z)
    struct Tri
    {
      int m_index[3]; // Each element is an index into an array of Verts
      int m_uvIndex[3]; // Each element is an index into an array of UVs
      Vec3 m_normals[3];
    };
    std::vector<Tri> m_indexes;
  };

  struct UV
  {
    UV() {}
    UV(float u, float v) : m_u(u), m_v(v) {}
    float m_u, m_v;
  };
  typedef std::vector<UV> UVs;

  // Call once at app startup
  static void Init();

  // Call to create window
  static bool CreateWindow(AmjuGLWindowInfo*);

  // Call before drawing anything
  static void BeginScene();

  // Call when all drawing finished
  static void EndScene();

  // Call to flip back/front buffers
  static void Flip();

  // Set viewport as screen coords
  static void Viewport(int x, int y, int w, int h);

  // Set rotation, e.g. when rotating iPhone from landscape to portrait.
  // You will also need to adjust aspect ratio and probably the controls!!
  static void SetScreenRotation(float degs);
  static float GetScreenRotation();

  // Call at start of drawing every frame.
  // Specify clear colour
  static void SetClearColour(const Colour& col);

  static void InitFrame();

  // Set up perspective projection matrix, by specifying field-of-view 
  //  and aspect ratio, and distances to the near and far planes (> 0).
  static void SetPerspectiveProjection(
    float fov, float aspectRatio, float near, float far);

  // Set 'camera': give position of eye, target position and Up vector
  static void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ);

  // Set the current colour
  static void SetColour(float r, float g, float b, float a = 1.0f);
  static void SetColour(const Colour& colour);

  // Draw line in current colour between 2 absolute coords
  static void DrawLine(const Vec3& v1, const Vec3& v2);

  // Draw quad with the 4 vertices
//  static void DrawQuad(Vert* verts);
  // TODO Replace with 

  // Draw a list of quads - would be more efficient for e.g. font, particle quads
//  static void DrawQuadList(const Quads& quads);

  // Draw list of triangles
  static void DrawTriList(const Tris& tris);

  // Draw list of triangles using vertex indices - more efficient as data is
  //  more compact
  static void DrawIndexedTriList(
    const Verts& verts,
    const IndexedTriList& indexes);

#ifdef _DEBUG
  static bool IsWireFrameMode();
  static void SetIsWireFrameMode(bool);
#endif

  // Set current matrix stack
  // TODO Can we use Texture matrix in GL ES/D3D ?
  enum MatrixMode { 
    AMJU_MATRIX_NOTSETYET  = -1, 
    AMJU_MODELVIEW_MATRIX  =  0, 
    AMJU_PROJECTION_MATRIX =  1, 
    AMJU_TEXTURE_MATRIX    =  2};

  static void SetMatrixMode(MatrixMode);
  static MatrixMode GetMatrixMode();

  // Set the current Matrix to the identity
  static void SetIdentity();

  // Push/pop current matrix stack
  static void PushMatrix();
  static void PopMatrix();

  // Translate/Scale/Rotate top of current matrix stack
  static void Translate(float x, float y, float z);

  static void Scale(float x, float y, float z);

  static void RotateX(float degs);
  static void RotateY(float degs);
  static void RotateZ(float degs);

  // Get the current value of the given matrix
  static void GetMatrix(MatrixMode, float result[16]);

  // Set the given matrix to the given values
  // TODO
  //static void SetMatrix(MatrixMode, float result[16]);

  // Multiply the currently selected matrix by the given matrix
  static void MultMatrix(const float matrix[16]);

  static void PushAttrib(uint32 attrib);
  static void PopAttrib();

  static void Enable(uint32 flags);
  static void Disable(uint32 flags);


  // Texture management

  // Used to refer to texture 
  typedef unsigned int TextureHandle;

  // Call to delete Texture handle
  static void DestroyTextureHandle(TextureHandle*);

  enum TextureType { AMJU_TEXTURE_REGULAR, AMJU_TEXTURE_SPHERE_MAP };

  enum TextureDepth { AMJU_RGB, AMJU_RGBA };

  // Set up a texture: specify type, depth, width and height, and give data.
  // TextureHandle is set on return.
  static void SetTexture(
    TextureHandle*, 
    TextureType, 
    TextureDepth, 
    int width, 
    int height, 
    uint8* data);

  // Call to use a Texture once set up
  static void UseTexture(TextureHandle);

  static void SetTextureMode(TextureType tt);

  // Copy screen into buffer  - which should be allocated by caller.
  // Format of data is RGB888
  static void GetScreenshot(unsigned char* buffer, int w, int h);

  // Lighting

  struct LightColour  
  {
    LightColour() {}
    LightColour(float r, float g, float b) : m_r(r), m_g(g), m_b(b) {}

    float m_r, m_g, m_b;
  };

  static void DrawLighting(
    const LightColour& globalAmbient, 
    const LightColour& lightAmbient,
    const LightColour& lightDiffuse,
    const LightColour& lightSpecular,
    const Vec3& lightPos);

  static Shader* LoadShader(const std::string& shaderFileName);

  // Create a ShadowMap (ref counted, on heap).
  static ShadowMap* CreateShadowMap();

}; // struct AmjuGL
} // namespace Amju

#endif

