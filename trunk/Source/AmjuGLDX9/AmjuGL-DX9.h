/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#ifndef AMJU_GL_DX9_H_INCLUDED
#define AMJU_GL_DX9_H_INCLUDED

#include <d3d9.h>
#include <D3DX9Effect.h>
#include "AmjuGL-Impl.h"

namespace Amju
{
class AmjuGLDX9 : public AmjuGLImpl
{
public:
  AmjuGLDX9(LPDIRECT3DDEVICE9 d3dDevice);

  // Call before drawing anything
  virtual void BeginScene();

  // Call when all drawing finished: Impl must swap buffers
  virtual void EndScene();

  // Set viewport as screen coords
  virtual void Viewport(int x, int y, int w, int h);

  // Call once at app startup
  virtual void Init();

  // Call at start of drawing every frame.
  // Specify clear colour
  virtual void InitFrame(float clearR, float clearG, float clearB);

  // Set up projection matrix, by specifying field-of-view and aspect ratio.
  // Near and far planes are currently hardcoded.
  virtual void SetPerspectiveProjection(float fov, float aspectRatio);

  virtual void SetOrthoProjection();

  // Set 'camera': give position of eye, target position and Up vector
  virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ);

  // Set the current colour
  virtual void SetColour(float r, float g, float b, float a = 1.0f);

  // Draw line in current colour between 2 absolute coords
  virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2);

  // Draw sphere (for debugging) at absolute coord v with radius r
  virtual void DrawSphere(const AmjuGL::Vec3& v, float r);

  // Draw quad with the 4 vertices
  virtual void DrawQuad(AmjuGL::Vert* verts);

  virtual void DrawQuadList(const AmjuGL::Quads& quads);

  virtual void DrawTriList(const AmjuGL::Tris& tris);

  virtual void DrawIndexedTriList(
    const AmjuGL::Verts& verts,
    const AmjuGL::IndexedTriList& indexes);


  // Set current matrix stack
  virtual void SetMatrixMode(AmjuGL::MatrixMode);

  // Set the current Matrix to the identity
  virtual void SetIdentity();

  // Push/pop current matrix stack
  virtual void PushMatrix();
  virtual void PopMatrix();

  // Translate/Scale/Rotate top of current matrix stack
  virtual void Translate(float x, float y, float z);
  virtual void Scale(float x, float y, float z);
  virtual void RotateX(float degs);
  virtual void RotateY(float degs);
  virtual void RotateZ(float degs);

  // Get the current value of the given matrix
  virtual void GetMatrix(AmjuGL::MatrixMode, float result[16]);

  virtual void PushAttrib(uint32 attrib);
  virtual void PopAttrib();

  virtual void Enable(uint32 flags);
  virtual void Disable(uint32 flags);

  virtual void BlendFunc();

  // Set Depth Mask: i.e. whether we should write to the z buffer.
  virtual void EnableZWrite(bool);


  // Call to set up a new Texture handle
  //virtual void CreateTextureHandle(AmjuGL::TextureHandle*);

  // Call to delete Texture handle
  virtual void DestroyTextureHandle(AmjuGL::TextureHandle*);

  virtual void SetTexture(
    AmjuGL::TextureHandle*, 
    AmjuGL::TextureType, 
    AmjuGL::TextureDepth, 
    int width, 
    int height, 
    uint8* data);

  // Call to use a Texture once set up
  virtual void UseTexture(AmjuGL::TextureHandle);

  virtual void SetTextureMode(AmjuGL::TextureType tt);

  // Get point int world coords from mouse cursor pos.
  virtual AmjuGL::Vec3 MouseToWorld(int mouseX, int mouseY);

  // Copy screen into buffer  - which should be allocated by caller.
  virtual void GetScreenshot(unsigned char* buffer, int w, int h);

  // Lighting
  virtual void DrawLighting(
    const AmjuGL::LightColour& globalAmbient, 
    const AmjuGL::LightColour& lightAmbient,
    const AmjuGL::LightColour& lightDiffuse,
    const AmjuGL::LightColour& lightSpecular,
    const AmjuGL::Vec3& lightPos);

}; // class 

class DX9Shader
{
public:
  DX9Shader();
  ~DX9Shader();

  bool Load(const std::string& fxFileName);

  std::string GetErrorString();

  // Start using the shader. Use Begin before any pass
  void Begin();

  // Finish using the shader: call after all passes are finished
  void End();

  // After calling begin, you can get the number of passes for this effect
  int GetNumPasses() const;

  // Use BeginPass/EndPass around each render pass
  void BeginPass(int passNum);
  void EndPass();

private:
  ID3DXEffect* m_pEffect;
  unsigned int m_numPasses;
};

} // namespace Amju

#endif

