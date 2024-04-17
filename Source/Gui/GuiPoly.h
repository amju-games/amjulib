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

protected:
  virtual bool LoadPoints(File*);
  virtual bool SavePoints(File*);

  bool ParseAttribString(const std::string& attribStr);

  // Parse one attrib: could be a single string, or two, which were split with '='.
  virtual bool ParseOneAttrib(const Strings& strs);
  virtual std::string CreateAttribString() const;

  virtual void BuildTriList();
  virtual void BuildOutlineTriList() = 0;
  virtual void BuildFilledTriList() = 0;

  RCPtr<TriListStatic> m_triList;
  ControlPoints m_controlPoints; // corners or control points for a spline
  bool m_isLoop = true; // True for filled polys, can be false for outline polys and splines
  Colour m_filledColour;
  Colour m_outlineColour;
  Style m_style = Style::FILLED_AND_OUTLINE;
  float m_cornerRadius = 0.f;
};

class GuiPoly : public IGuiPoly
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

protected:
  void BuildFilledTriList() override;
  void BuildOutlineTriList() override;
};
}