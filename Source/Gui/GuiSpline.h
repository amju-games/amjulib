// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#pragma once

#include <FunctionFactory.h>
#include <Texture.h>
#include <TriList.h>
#include "GuiPoly.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI

namespace Amju
{
// WIDTH FUNCTIONS
// Define how the curve width varies with t, given two widths.

// Simple lerp
static inline float LerpWidthFunc(float t, float w1, float w2)
{
  return std::lerp(w1, w2, t);
}

static inline float TunableWidthFunc(float t, float w1, float w2, float exponent)
{
  // Clamp t just in case to avoid any issues with fractional powers
  float clamped_t = std::fmax(0.0f, std::fmin(1.0f, t));

  // Warp t: exponent < 1 is log-like, exponent > 1 is exp-like
  float warped_t = std::pow(clamped_t, exponent);

  warped_t = std::clamp(warped_t, 0.f, 1.f);
  return std::lerp(w1, w2, warped_t);
}

static inline float LogWidthFunc(float t, float w1, float w2)
{
  return TunableWidthFunc(w1, w2, t, .9f);
}

static inline float ExpWidthFunc(float t, float w1, float w2)
{
  return TunableWidthFunc(w1, w2, t, 3.f);
}

// For music curves: vary from w1 at the ends to w2 in the middle.
static inline float MusicCurveWidthFunc(float t, float w1, float w2)
{
  // d is a half sine wave
  const float d = sin(t * static_cast<float>(M_PI));
  // Square d to get a bell shape
  return std::lerp(w1, w2, d * d);
}

// * GuiSpline *
// Curved line, passes through a vector of control points (Catmull-Rom spline).
// Can be 'animated' in the sense of showing a portion of the spline, varying from
//  0..1 (i.e. the parametric 't' value).
// As we travel from 0 to 1, the width varies according to the width function
//  set. If not set, we use the default width function.
class GuiSpline : public IGuiPoly
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }
  GuiSpline();

  GuiSpline* Clone() override { return new GuiSpline(*this); }

  void Animate(float animValue) override;

  void OnControlPointsChanged() override;

  void SetWidths(float w1, float w2);

  // Width function: specifies how the width varies with parametric t (0..1).
  // Returns width for given t, with w1 and w2 passed in.
  using WidthFunc = std::function<float(float t, float w1, float w2)>;
  void SetWidthFunc(WidthFunc wf) { m_widthFunc = wf; }

  // Create a WidthFunc from given string; if string not recognised, 
  //  returns nullptr and str is cleared. Hence pass by non-const ref.
  // (That way, we don't save a bad name.)
  static WidthFunc WidthFuncFactoryCreate(std::string& str);

  // Add a new WidthFunc to our tiny factory.
  static bool AddWidthFunc(const std::string& str, WidthFunc f);

protected:
  void MakeInBetweenPoints();
  AmjuGL::Tris BuildFilledTriList() override;
  AmjuGL::Tris BuildOutlineTriList() override;

  std::string CreateAttribString() const override;
  bool ParseOneAttrib(const Strings& strs) override;

protected:
  // All points, after we interpolate between the control points with
  //  a Catmull-Rom spline.
  std::vector<Vec2f> m_points;

  // Alpha param in Catmull-Rom formula
  float m_alpha = .5f;

  // Number of interpolated points between control points
  int m_numPoints = 5;

  // Total length of all segments 
  float m_totalLength = 0;

  int m_index = 0; // index into m_points

  float m_startWidth = 0.03f;
  float m_endWidth = 0.01f;

  // Can't use 'using'
  #define DefaultWidthFunction MusicCurveWidthFunc

  WidthFunc m_widthFunc = DefaultWidthFunction;
  // So we can load/save function name and look up in factory
  std::string m_widthFuncName;

  // Width Func Factory 
  static FunctionFactory<WidthFunc> s_widthFuncFactory;
};
}


