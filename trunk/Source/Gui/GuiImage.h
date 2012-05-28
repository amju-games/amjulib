#ifndef GUI_IMAGE_H
#define GUI_IMAGE_H

#include "GuiElement.h"
#include "Texture.h"

namespace Amju
{
class GuiImage : public GuiElement
{
public:
  static const char* NAME;

  virtual void Draw();
  virtual bool Load(File*);

  void SetTexture(Texture* tex);

private:
  void BuildTris();

protected:
  PTexture m_texture;
  AmjuGL::Tris m_tris;
  Vec2f m_oldPos;
  Vec2f m_oldSize;
};
}

#endif
