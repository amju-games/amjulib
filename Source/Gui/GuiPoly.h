#pragma once

#include <Colour.h>
#include <StringUtils.h>
#include <TriList.h>
#include "GuiElement.h"

namespace Amju
{
class IGuiPoly : public GuiElement
{
public:
  void Draw() override;
  bool Load(File*) override;
  bool Save(File*) override;

  void SetFilledColour(const Colour& colour);
  const Colour& GetFilledColour() const;

  void SetOutlineColour(const Colour& colour);
  const Colour& GetOutlineColour() const;

  using ControlPoints = std::vector<Vec2f>;
  const ControlPoints& GetControlPoints() const;
  ControlPoints& GetControlPoints();
  void AddControlPoint(const Vec2f& p);
  virtual void OnControlPointsChanged(); // Called from Editor

  // Create a GuiElement editor object which lets us edit this node type.
  GuiEdit* CreateEditor() override;

  bool IsLoop() const;
  void SetIsLoop(bool isLoop);

  Rect CalcRect() const override;

  enum class Style
  {
    FILLED = 1,
    OUTLINE = 2,
    FILLED_AND_OUTLINE = 3
  };
  Style GetStyle() const;
  void SetStyle(Style s);
  bool IsFilled() const;
  bool IsOutline() const;

  void CreateEditorDefault() override;

protected:
  virtual bool LoadPoints(File*);
  virtual bool SavePoints(File*);

  bool ParseAttribString(const std::string& attribStr);

  // Parse one attrib: could be a single string, or two, which were split with '='.
  virtual bool ParseOneAttrib(const Strings& strs);
  virtual std::string CreateAttribString() const;

  virtual void BuildTriList();
  virtual AmjuGL::Tris BuildOutlineTriList() = 0;
  virtual AmjuGL::Tris BuildFilledTriList() = 0;

  RCPtr<TriListStatic> m_triList;
  ControlPoints m_controlPoints; // corners or control points for a spline
  bool m_isLoop = true; // True for filled polys, can be false for outline polys and splines
  Colour m_filledColour{ 1, 0, 0, 1 };
  Colour m_outlineColour{ 0, 1, 1, 1 };
  Style m_style = Style::FILLED_AND_OUTLINE;
  float m_cornerRadius = 0.1f;

  // TODO Move these up the hierarchy, so we can batch tri lists of as many
  //  GUI elements as poss.
  Vec2f m_previousCombinedPos;
  Vec2f m_previousCombinedSize;
  Colour m_previousCombinedColour;
};

class GuiPoly : public IGuiPoly
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }
  
  GuiPoly* Clone() override { return new GuiPoly(*this); }

protected:
  AmjuGL::Tris BuildFilledTriList() override;
  AmjuGL::Tris BuildOutlineTriList() override;
};
}