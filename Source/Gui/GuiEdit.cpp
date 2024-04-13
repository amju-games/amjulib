#include <AmjuGL.h>
#include <DrawRect.h>
#include "GuiEdit.h"

namespace Amju
{
const char* GuiEdit::NAME = "gui-edit";

GuiEdit::GuiEdit()
{
  m_selectedGrabberIndex = -1;
  m_grabbers.clear();
  m_mouseButtonIsDown = false;
}

void GuiEdit::SetChild(GuiElement* child)
{
  m_children.clear();
  m_children.push_back(child);
  // Don't set parent of child to this: it already has a parent in its GUI tree (or it's the root).
  // We don't want to modify that structure. 
}

void GuiEdit::Draw()
{
  //GuiDecorator::Draw();
  DrawGrabbers();
}

void GuiEdit::HandleDragGrabber(const Vec2f& d)
{
  // Logic for standard, rectangular shape with corner grabbers.
  Vec2f pos = GetChild()->GetLocalPos();
  Vec2f size = GetChild()->GetSize();
  if (m_selectedGrabberIndex == 0) // Bottom left dragged
  {
    pos.x += d.x;
    size.x -= d.x;
    size.y -= d.y;
  }
  else if (m_selectedGrabberIndex == 1) // Bottom right
  {
    size.x += d.x;
    size.y -= d.y;
  }
  else if (m_selectedGrabberIndex == 2) // Top right
  {
    pos.y += d.y;
    size.x += d.x;
    size.y += d.y;
  }
  else if (m_selectedGrabberIndex == 3) // Top left
  {
    pos.x += d.x;
    pos.y += d.y;
    size.x -= d.x;
    size.y += d.y;
  }
  else if (m_selectedGrabberIndex == 4) // Bottom mid
  {
    size.y -= d.y;
  }
  else if (m_selectedGrabberIndex == 5) // Right mid
  {
    size.x += d.x;
  }
  else if (m_selectedGrabberIndex == 6) // Top mid
  {
    pos.y += d.y;
    size.y += d.y;
  }
  else if (m_selectedGrabberIndex == 7) // Left mid
  {
    pos.x += d.x;
    size.x -= d.x;
  }
  else if (m_selectedGrabberIndex == 8) // Centre
  {
    pos.x += d.x;
    pos.y += d.y;
  }

  GetChild()->SetLocalPos(pos);
  GetChild()->SetSize(size);
  RecalcGrabberPositions();
}

int GuiEdit::GetGrabberUnderCursor(const Vec2f& pos)
{
  for (int i = 0; i < static_cast<int>(m_grabbers.size()); i++)
  {
    if (m_grabbers[i].IsPointIn(pos))
    {
      //std::cout << "Grabber has been grabbed!\n";
      return i;
    }
  }
  return -1;
}

void GuiEdit::RecalcGrabberPositions()
{
  m_grabbers.clear();

  // Populate grabbers
  const Rect r = GetRect(GetChild());
  // Corners
  m_grabbers.push_back(Rect(r.GetMin(0) - GRAB_SIZE_X, r.GetMin(0) + GRAB_SIZE_X, r.GetMin(1) - GRAB_SIZE_Y, r.GetMin(1) + GRAB_SIZE_Y));
  m_grabbers.push_back(Rect(r.GetMax(0) - GRAB_SIZE_X, r.GetMax(0) + GRAB_SIZE_X, r.GetMin(1) - GRAB_SIZE_Y, r.GetMin(1) + GRAB_SIZE_Y));
  m_grabbers.push_back(Rect(r.GetMax(0) - GRAB_SIZE_X, r.GetMax(0) + GRAB_SIZE_X, r.GetMax(1) - GRAB_SIZE_Y, r.GetMax(1) + GRAB_SIZE_Y));
  m_grabbers.push_back(Rect(r.GetMin(0) - GRAB_SIZE_X, r.GetMin(0) + GRAB_SIZE_X, r.GetMax(1) - GRAB_SIZE_Y, r.GetMax(1) + GRAB_SIZE_Y));
  // Mids
  const float midx = (r.GetMin(0) + r.GetMax(0)) * .5f;
  const float midy = (r.GetMin(1) + r.GetMax(1)) * .5f;
  m_grabbers.push_back(Rect(midx - GRAB_SIZE_X, midx + GRAB_SIZE_X, r.GetMin(1) - GRAB_SIZE_Y, r.GetMin(1) + GRAB_SIZE_Y));
  m_grabbers.push_back(Rect(r.GetMax(0) - GRAB_SIZE_X, r.GetMax(0) + GRAB_SIZE_X, midy - GRAB_SIZE_Y, midy + GRAB_SIZE_Y));
  m_grabbers.push_back(Rect(midx - GRAB_SIZE_X, midx + GRAB_SIZE_X, r.GetMax(1) - GRAB_SIZE_Y, r.GetMax(1) + GRAB_SIZE_Y));
  m_grabbers.push_back(Rect(r.GetMin(0) - GRAB_SIZE_X, r.GetMin(0) + GRAB_SIZE_X, midy - GRAB_SIZE_Y, midy + GRAB_SIZE_Y));
  // Centre
  m_grabbers.push_back(Rect(midx - GRAB_SIZE_X, midx + GRAB_SIZE_X, midy - GRAB_SIZE_Y, midy + GRAB_SIZE_Y));
}

void GuiEdit::DrawGrabbers()
{
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  PushColour();
  for (int i = 0; i < static_cast<int>(m_grabbers.size()); i++)
  {
    const Rect& r = m_grabbers[i];
    AmjuGL::SetColour(Colour(0, 0, 1, 1));
    if (m_highlightedGrabber == i)
    {
      AmjuGL::SetColour(Colour(0, 1, 1, 1));
    }
    if (m_selectedGrabberIndex == i)
    {
      AmjuGL::SetColour(Colour(1, 0, 1, 1));
    }
    DrawSolidRect(r);
  }
  PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
}

bool GuiEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    m_mouseButtonIsDown = mbe.isDown;
  }

  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT &&
    mbe.isDown)
  {
    // Check for grabbing a control point/corner
    m_selectedGrabberIndex = GetGrabberUnderCursor(Vec2f(mbe.x, mbe.y));
    if (m_selectedGrabberIndex != -1)
    {
      return true;
    }

    // Unselect previously selected
    m_grabbers.clear();
  }
  else if (mbe.button == AMJU_BUTTON_MOUSE_LEFT &&
    !mbe.isDown)
  {
    // Has grabber moved? Save new position, as an undoable Command.
    m_selectedGrabberIndex = -1;
    return true;
  }

  return false;
}

bool GuiEdit::OnCursorEvent(const CursorEvent& ce)
{
  if (m_selectedGrabberIndex == -1)
  {
    // Search grabbers, highlight if we mouse over it
    m_highlightedGrabber = GetGrabberUnderCursor(Vec2f(ce.x, ce.y));
  }
  else
  {
    HandleDragGrabber(Vec2f(ce.dx, ce.dy));
    return true;
  }
  return false;
}

}