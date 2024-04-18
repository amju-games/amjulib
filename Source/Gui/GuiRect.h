#ifndef GUI_RECT_H_INCLUDED
#define GUI_RECT_H_INCLUDED

#include <AmjuGL.h>
#include <Colour.h>
#include <TriList.h>
#include "GuiElement.h"

namespace Amju
{
class GuiRect : public GuiElement
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiRect* Clone() override { return new GuiRect(*this); }

  GuiRect();
  virtual void Draw();
  virtual bool Load(File*);

  enum
  {
    AMJU_TL = 1, AMJU_BL = 2, AMJU_TR = 4, AMJU_BR = 8
  };

  void SetRoundCorners(unsigned int);
  void SetCornerRadius(float);
  void SetColour(const Colour& colour);

  static void SetCornerImage(const std::string &cornerImageFilename);

protected:
  void BuildTris();

protected:
  static std::string s_cornerImageFilename;

  RCPtr<TriListStatic> m_triList;
  float m_cornerRadius;
  Vec2f m_oldPos;
  Vec2f m_oldSize;
  Colour m_colour;
  unsigned int m_flags; // combo of enums: a 1 means 90 degree corner, 0 means round
};
}

#endif

