#include <Line2.h>
#include "GuiSpline.h"
#include "GuiPolyEdit.h"

namespace Amju
{
const char* GuiPolyEdit::NAME = "gui-spline-edit";

GuiPolyEdit::GuiPolyEdit()
{
} 

void GuiPolyEdit::RecalcGrabberPositions()
{
  m_grabbers.clear();

  // Central grabber, to reposition
  const Rect r = GetPoly()->CalcRect();
  const Vec2f q(r.GetCentre());
  m_grabbers.push_back(Rect(q.x - GRAB_SIZE_X, q.x + GRAB_SIZE_X, q.y - GRAB_SIZE_Y, q.y + GRAB_SIZE_Y));

  const Vec2f pos = GetPoly()->GetCombinedPos();

  const auto& points = GetPoly()->GetControlPoints();
  for (const auto& p : points)
  {
    const Vec2f q = pos + p;
    m_grabbers.push_back(Rect(q.x - GRAB_SIZE_X, q.x + GRAB_SIZE_X, q.y - GRAB_SIZE_Y, q.y + GRAB_SIZE_Y));
  }
}

void GuiPolyEdit::HandleDragGrabber(const Vec2f& deltaPos)
{
  if (m_selectedGrabberIndex == 0)
  {
    // Central grabber
    GetPoly()->SetLocalPos(GetPoly()->GetLocalPos() + deltaPos);
  }
  else
  {
    auto& points = GetPoly()->GetControlPoints();
    Assert(m_selectedGrabberIndex >= 0);
    Assert(m_selectedGrabberIndex <= static_cast<int>(points.size()));
    Vec2f& p = points[m_selectedGrabberIndex - 1];
    p += deltaPos;
  }

  RecalcGrabberPositions();
  GetPoly()->OnControlPointsChanged();
  // TODO This should recalc the bounding rect too.
}

IGuiPoly* GuiPolyEdit::GetPoly()
{
  IGuiPoly* s = dynamic_cast<IGuiPoly*>(GetChild());
  Assert(s);
  return s;
}

void GuiPolyEdit::Draw()
{
  GuiEdit::Draw();

  const auto& points = GetPoly()->GetControlPoints();
  int n = static_cast<int>(points.size());
  if (n < 2)
  {
    return; // can't draw
  }

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  PushColour();
  AmjuGL::SetColour(Colour(0, 0, 1, 1));

  //AmjuGL::SetIsWireFrameMode(true);
  ////GetPoly()->Draw();
  //AmjuGL::Draw(GetPoly()->m_triList);
  //AmjuGL::SetIsWireFrameMode(false);


  // Draw lines between control points, so we can see order
  const Vec2f pos = GetPoly()->GetCombinedPos();

  for (int i = 1; i < n; i++)
  {
    const Vec2f q0 = points[i - 1] + pos;
    const Vec2f q1 = points[i] + pos;
    AmjuGL::DrawLine(AmjuGL::Vec3(q0.x, q0.y, 0), AmjuGL::Vec3(q1.x, q1.y, 0));
  }

  if (GetPoly()->IsLoop())
  {
    const Vec2f q0 = points[n - 1] + pos;
    const Vec2f q1 = points[0] + pos;

    AmjuGL::SetColour(Colour(0, 1, 1, .5)); // Show this is the loop closing line seg
    AmjuGL::DrawLine(AmjuGL::Vec3(q0.x, q0.y, 0), AmjuGL::Vec3(q1.x, q1.y, 0));
  }

  PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
}

void GuiPolyEdit::DeleteHighlightedControlPoint()
{
  auto& points = GetPoly()->GetControlPoints();
  Assert(m_highlightedGrabber >= 0);
  Assert(m_highlightedGrabber < static_cast<int>(points.size()));
  points.erase(points.begin() + m_highlightedGrabber);
  RecalcGrabberPositions();
  GetPoly()->OnControlPointsChanged();
}

bool GuiPolyEdit::CreateNewControlPoint(const MouseButtonEvent& mbe)
{
  // See if we clicked on a line seg between two control points.
  const float MIN_SQ_DIST = 0.002f;
  const Vec2f p(mbe.x, mbe.y);
  const Vec2f pointsOffset = GetPoly()->GetCombinedPos();
  auto& points = GetPoly()->GetControlPoints();
  for (int i = 1; i < static_cast<int>(points.size()); i++)
  {
    const Vec2f q0 = points[i - 1] + pointsOffset;
    const Vec2f q1 = points[i] + pointsOffset;
    const Line2<float> line(q0, q1);
    const float sqDist = line.SqDist(p);
    if (sqDist < MIN_SQ_DIST)
    {
      // Close enough! Add a new control point.
      points.insert(points.begin() + i, p - pointsOffset);
      GetPoly()->OnControlPointsChanged();
      RecalcGrabberPositions();
      return true;
    }
  }
  // Handle adding point when we click on line connecting front and back control points
  if (GetPoly()->IsLoop()) // ?
  {
    const Vec2f q0 = points.front() + pointsOffset;
    const Vec2f q1 = points.back() + pointsOffset;
    const Line2<float> line(q0, q1);
    const float sqDist = line.SqDist(p);
    if (sqDist < MIN_SQ_DIST)
    {
      // Nearer front or back?
      // Add a new control point at start or end.
      const Vec2f m(q1 - q0);
      const float mDotM = DotProduct(m, m);
      const float EPSILON = 0.000001f;
      if (fabs(mDotM) < EPSILON)
      {
        return false;
      }
      const float t = DotProduct(m, (p - q0)) / mDotM;
      if (t < .5f)
      {
        points.insert(points.begin(), p - pointsOffset);
      }
      else
      {
        points.push_back(p - pointsOffset);
      }
      GetPoly()->OnControlPointsChanged();
      RecalcGrabberPositions();
      return true;
    }
  }

  return false;
}

bool GuiPolyEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT)
  {
    // Right click on grabber: menu to delete it? Or keypress to delete highlighed grabber?
    if (mbe.isDown && m_highlightedGrabber != -1)
    {
      DeleteHighlightedControlPoint();
      return true;
    }

    // Right click NOT on a grabber: add a new control point? Hmm, we need to know where it goes in sequence.
    //  So you have to click on a line between 2 existing control points.

    // If you R click down on (or close enough to) a line seg joining two control points,
    //  add a new control point where you clicked.
    if (mbe.isDown &&
      CreateNewControlPoint(mbe))
    {
      return true;
    }

    return false;
  }
  else
  {

    return GuiEdit::OnMouseButtonEvent(mbe);
  }
}

}