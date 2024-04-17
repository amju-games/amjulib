#pragma once

#include <GuiEdit.h>

namespace Amju
{
class GuiSpline;

// Editor for GuiSplines; special case of general purpose editor.
class GuiPolyEdit : public GuiEdit
{
public:
  GuiPolyEdit();

  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }
  void Draw() override;
  bool OnMouseButtonEvent(const MouseButtonEvent& mbe) override;

  void RecalcGrabberPositions() override;

protected:
  // Returns true if mouse cursor is on line between two control points.
  // If so, we add a new control point at the coord, in the expected
  //  place in the sequence of control points.
  bool CreateNewControlPoint(const MouseButtonEvent& mbe);

  // Delete the currently highlighted grabber/control point.
  void DeleteHighlightedControlPoint();

protected:
  void HandleDragGrabber(const Vec2f& deltaPos) override;

  IGuiPoly* GetPoly();
};
}
