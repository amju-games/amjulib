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
  void AddCheckBox();
};

// Convenience: add a tweakable to the given GUI dialog
void AddTweakable(GuiDialog* dialog, Tweakable* tweak);

class TweakableBool : public Tweakable
{
public:
  TweakableBool(const std::string& label, bool* var);
  void SetVar(bool b) { *m_var = b; }

protected:
  bool* m_var;
};

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


