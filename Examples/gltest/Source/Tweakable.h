#pragma once

#include <GuiComposite.h>
#include <GuiDialog.h>

namespace Amju
{
// Add this to a gui element, e.g. a dialog box. When the value is changed,
//  we update the variable we point to.
class Tweakable : public GuiComposite
{
public:

protected:
  void SetLabel(const std::string& label);
  void AddEditBox();  
};

// Convenience: add a tweakable to the given GUI dialog
void AddTweakable(GuiDialog* dialog, Tweakable* tweak);

// Or templates?
class TweakableFloat : public Tweakable
{
public:
  TweakableFloat(const std::string& label, float* var, float min, float max);

protected:
  float* m_var;
  float m_min;
  float m_max;
};
}

