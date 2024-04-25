#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "GuiImage.h"
#include "GuiText.h"
#include "Font.h"
#include "Colour.h"

namespace Amju
{
class GuiButton : public GuiImage
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiButton* Clone() override { return new GuiButton(*this); }

  GuiButton();
  ~GuiButton();

  // GuiElement overrides
  bool Load(File*) override; 
  bool Save(File*) override;
  void Draw() override;
  void OnGetFocus() override;
  void TextToSpeech() override;

  // EventListener overrides - respond to mouse/cursor movement
  //  and button presses
  bool OnCursorEvent(const CursorEvent&) override;
  bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  bool OnButtonEvent(const ButtonEvent&) override;
  bool OnKeyEvent(const KeyEvent&) override; // Enter triggers if this is the focus button

  void SetText(const std::string& text);
  void SetTextColour(const Colour& col);
  const Colour& GetTextColour() const;
  void SetButtonColour(const Colour& col);
  const Colour& GetButtonColour() const;
  void SetFocusColour(const Colour& col);
  const Colour& GetFocusColour() const;

  bool IsMouseOver() const;
  bool IsPressed() const;

  bool IsFocusButton() const;
  void SetIsFocusButton(bool isFocusButton);
  void SetShowIfFocus(bool);

  bool IsCancelButton() const;
  void SetIsCancelButton(bool isCancelButton);

  virtual bool IsEnabled() const;
  void SetIsEnabled(bool);

  void SetOnPressedDownFunc(CommandFunc f);
  static void SetClickFilename(const std::string& clickFilename);

protected:
  void ClickSound() const; // TODO in GuiElement ?
  void SetIsMouseOver(bool);
  void OnPressedDown();

protected:
  GuiText m_guiText;
  Colour m_buttonColour;
  Colour m_focusColour;

  bool m_isMouseOver; // Draw bigger
  bool m_isPressed; // Draw offset right/down
  bool m_isEnabled;
  bool m_showIfFocus;

  CommandFunc m_onPressedDown;

  GuiImage m_focusImage;
  
  static std::string s_clickFilename;
  
  Colour m_combinedColour; // combined colour of this button and all ancestors
};
}

#endif
