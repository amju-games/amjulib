// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2017

#pragma once

#include <GuiElement.h>
#include <Texture.h>
#include <TriList.h>

namespace Amju
{
// * GuiImage *
// Part of Gui class hierarchy, displays 2D screen space GUI image.
class GuiImage : public GuiElement
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  virtual void Draw() override;
  virtual bool Load(File*) override;
  virtual bool Save(File*) override;

  void SetTexture(Texture* tex);
  Texture* GetTexture();

private:
  // Build (2 element) tri list to draw the textured quad.
  // NB this is super inefficient: GuiSprite batches draw calls better,
  //  especially if the images are all in one atlas.
  virtual void BuildTris();

protected:
  PTexture m_texture;
  RCPtr<TriListStatic> m_triList;

  std::string m_textureName;

  // If true, image is stretched to the element dimensions; if false, we modify the height to 
  //  preserve the image aspect ratio.
  bool m_stretch = false; 

  // If true, use 'nearest' texture filtering, as opposed to 'nicest', the default.
  bool m_nearest = false;
};
}
