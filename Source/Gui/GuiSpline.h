// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiPoly.h>
#include <Texture.h>
#include <TriList.h>

namespace Amju
{
// * GuiSpline *
// Curved line, passes through a vector of control points (Catmull-Rom spline).
// Can be 'animated' in the sense of showing a portion of the spline, varying from
//  0..1 (i.e. the parametric 't' value).
class GuiSpline : public IGuiPoly
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiSpline* Clone() override { return new GuiSpline(*this); }

  void Animate(float animValue) override;

  void OnControlPointsChanged() override;

  void SetWidths(float w1, float w2);

protected:
  void Reset(); // Reset animation

  void MakeInBetweenPoints();
  AmjuGL::Tris BuildFilledTriList() override;
  AmjuGL::Tris BuildOutlineTriList() override;

  std::string CreateAttribString() const override;
  bool ParseOneAttrib(const Strings& strs) override;

protected:
  // All points, after we interpolate between the control points with a Catmull-Rom spline.
  std::vector<Vec2f> m_points;

  // Total length of all segments 
  float m_totalLength = 0;

  int m_index = 0; // index into m_points

  float m_startWidth = 0.03f;
  float m_endWidth = 0.01f;
};
}


