#pragma once

#include "GuiButton.h"

namespace Amju
{
class GuiCheck;

// Callback for when check box value changes.
// Passes the check box and new value.
typedef void (*ChangeValueFunc)(GuiCheck*, bool);

// Check box
class GuiCheck : public GuiButton
{
public:
  static const char* NAME;
 
  GuiCheck();
  virtual bool IsEnabled() const override;

  void Set(Texture* checkedTex, Texture* uncheckedTex);
  void SetValue(bool b);
  bool GetValue() const { return m_value; }
  void Draw() override;

  void SetOnChangeValue(ChangeValueFunc f) { m_changeValueFunc = f; }

protected:
  void ExecuteCommand() override;

protected:
  Texture* m_checkedTex;
  Texture* m_uncheckedTex;
  ChangeValueFunc m_changeValueFunc;
  bool m_value;
};
}


