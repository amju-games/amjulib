
#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// This node type lets you edit the child GUI element.
class GuiEdit : public GuiDecorator
{
public:
  GuiEdit();

  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }
  void Draw() override;

  bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  bool OnCursorEvent(const CursorEvent&) override;

  void SetChild(GuiElement* child);

  virtual void RecalcGrabberPositions();

protected:
  void DrawGrabbers();

  // If pos is within a grabber, return index of selected grabber, else return -1.
  int GetGrabberUnderCursor(const Vec2f& pos);

  // Called when a grabber is dragged
  virtual void HandleDragGrabber(const Vec2f& deltaPos);

protected:
  using Grabbers = std::vector<Rect>;
  Grabbers m_grabbers;
  int m_selectedGrabberIndex = -1;
  int m_highlightedGrabber = -1; // Highlight grabber if under mouse cursor
  bool m_mouseButtonIsDown = false;

  const float GRAB_SIZE_X = 0.01f; // size of grabber in x
  const float GRAB_SIZE_Y = 0.02f; // size of grabber in y
};
}