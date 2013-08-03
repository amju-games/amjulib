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

  GuiButton();
  ~GuiButton();

  // GuiElement overrides
  virtual bool Load(File*); 
  virtual void Draw();
  virtual void OnGetFocus();
  virtual void TextToSpeech();

  // EventListener overrides - respond to mouse/cursor movement
  //  and button presses
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnButtonEvent(const ButtonEvent&);
  virtual bool OnKeyEvent(const KeyEvent&); // Enter triggers if this is the focus button

  void SetText(const std::string& text);
  void SetTextColour(const Colour& col);
  void SetButtonColour(const Colour& col);

  bool IsMouseOver() const;
  bool IsPressed() const;

  bool IsFocusButton() const;
  void SetIsFocusButton(bool isFocusButton);
  void SetShowIfFocus(bool);

  bool IsCancelButton() const;
  void SetIsCancelButton(bool isCancelButton);

  bool IsEnabled() const;
  void SetIsEnabled(bool);

  void SetOnPressedDownFunc(CommandFunc f);
  static void SetClickFilename(const std::string& clickFilename);

private:
  void ClickSound() const; // TODO in GuiElement ?
  void SetIsMouseOver(bool);
  void OnPressedDown();

protected:
  GuiText m_guiText;
  Colour m_textColour;
  Colour m_buttonColour;

  bool m_isMouseOver; // Draw bigger
  bool m_isPressed; // Draw offset right/down
  bool m_isEnabled;
  bool m_showIfFocus;

  CommandFunc m_onPressedDown;

  GuiImage m_focusImage;
  
  static std::string s_clickFilename;
};
}

#endif
