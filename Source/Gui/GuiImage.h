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

  virtual void Draw();
  virtual bool Load(File*);

  void SetTexture(Texture* tex);
  Texture* GetTexture();

private:
  void BuildTris();

protected:
  PTexture m_texture;
  RCPtr<TriListStatic> m_triList;
};
}
