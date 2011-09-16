/*
Amju Games source code (c) Copyright Jason Colman 2009
*/

#ifndef AMJU_GL_GCUBE_INCLUDED
#define AMJU_GL_GCUBE_INCLUDED

#include <map>
#include <gccore.h>
#include "AmjuGL-Impl.h"
#include "AmjuGLWindowInfo.h"

namespace Amju
{
class AmjuGLGCubeConsole : public AmjuGLImpl
{
public:
  AmjuGLGCubeConsole();
  virtual void Init();
  virtual bool CreateWindow(AmjuGLWindowInfo*) { return true; }
  virtual void BeginScene() {}
  virtual void EndScene() {}
  virtual void Flip();
  virtual void Viewport(int x, int y, int w, int h) {}
  virtual void InitFrame(float clearR, float clearG, float clearB) {}
  virtual void SetPerspectiveProjection(float fov, float aspectRatio, float nearDist, float farDist) {}
  virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ) {}
  virtual void SetColour(float r, float g, float b, float a = 1.0f) {}
  virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2) {}
  virtual void DrawTriList(const AmjuGL::Tris& tris) {}
  virtual void DrawIndexedTriList(
    const AmjuGL::Verts& verts,
    const AmjuGL::IndexedTriList& indexes) {}
  virtual void SetMatrixMode(AmjuGL::MatrixMode) {}
  virtual void SetIdentity() {}
  virtual void PushMatrix() {}
  virtual void PopMatrix() {}
  virtual void Translate(float x, float y, float z) {}
  virtual void Scale(float x, float y, float z) {}
  virtual void RotateX(float degs) {}
  virtual void RotateY(float degs) {}
  virtual void RotateZ(float degs) {}
  virtual void GetMatrix(AmjuGL::MatrixMode, float result[16]) {}
  virtual void MultMatrix(const float matrix[16]) {}
  virtual void PushAttrib(uint32 attrib) {}
  virtual void PopAttrib() {}
  virtual void Enable(uint32 flags) {}
  virtual void Disable(uint32 flags) {}
  virtual void BlendFunc() {}
  virtual void DestroyTextureHandle(AmjuGL::TextureHandle*) {}
  virtual void SetTexture(
    AmjuGL::TextureHandle*, 
    AmjuGL::TextureType, 
    AmjuGL::TextureDepth, 
    int width, 
    int height, 
    uint8* data) {}
  virtual void UseTexture(AmjuGL::TextureHandle) {}
  virtual void SetTextureMode(AmjuGL::TextureType tt) {}
  virtual void GetScreenshot(unsigned char* buffer, int w, int h) {}
  virtual void DrawLighting(
    const AmjuGL::LightColour& globalAmbient, 
    const AmjuGL::LightColour& lightAmbient,
    const AmjuGL::LightColour& lightDiffuse,
    const AmjuGL::LightColour& lightSpecular,
    const AmjuGL::Vec3& lightPos) {}
  virtual Shader* LoadShader(const std::string& shaderFileName) { return 0; }
};

class AmjuGLGCube : public AmjuGLImpl
{
public:
  AmjuGLGCube();

  virtual bool CreateWindow(AmjuGLWindowInfo*);

  // Call before drawing anything
  virtual void BeginScene();

  // Call when all drawing finished: Impl must swap buffers
  virtual void EndScene();

  virtual void Flip();

  // Set viewport as screen coords
  virtual void Viewport(int x, int y, int w, int h);

  // Call once at app startup
  virtual void Init();

  // Call at start of drawing every frame.
  // Specify clear colour
  virtual void InitFrame(float clearR, float clearG, float clearB);

  // Set up projection matrix, by specifying field-of-view and aspect ratio.
  // Near and far planes are currently hardcoded.
  virtual void SetPerspectiveProjection(
    float fov, float aspectRatio, float nearDist, float farDist);

  // Set 'camera': give position of eye, target position and Up vector
  virtual void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ);

  // Set the current colour
  virtual void SetColour(float r, float g, float b, float a = 1.0f);

  // Draw line in current colour between 2 absolute coords
  virtual void DrawLine(const AmjuGL::Vec3& v1, const AmjuGL::Vec3& v2);

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

  virtual void MultMatrix(const float matrix[16]);

  virtual void PushAttrib(uint32 attrib);
  virtual void PopAttrib();

  virtual void Enable(uint32 flags);
  virtual void Disable(uint32 flags);

  virtual void BlendFunc();

  // Set Depth Mask: i.e. whether we should write to the z buffer.
  virtual void EnableZWrite(bool);


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

  // Copy screen into buffer  - which should be allocated by caller.
  virtual void GetScreenshot(unsigned char* buffer, int w, int h);

  // Lighting
  virtual void DrawLighting(
    const AmjuGL::LightColour& globalAmbient, 
    const AmjuGL::LightColour& lightAmbient,
    const AmjuGL::LightColour& lightDiffuse,
    const AmjuGL::LightColour& lightSpecular,
    const AmjuGL::Vec3& lightPos);

  virtual Shader* LoadShader(const std::string& shaderFileName);

private:
  int m_texId; // to get unique texture IDs

  // Store texture data, free it when we are done with texture object
  struct TexData
  {
    TexData() : m_texObj(0), m_data(0) {}
    TexData(GXTexObj* texObj, unsigned char* data) : m_texObj(texObj), m_data(data) {} 
    GXTexObj* m_texObj;
    unsigned char* m_data; // free when texture object destroyed
  };
  typedef std::map<int, TexData> Textures;
  Textures m_textures;

}; // class AmjuGLGCube


} // namespace Amju

#endif

