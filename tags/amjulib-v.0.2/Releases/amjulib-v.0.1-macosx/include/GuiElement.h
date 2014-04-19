#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <vector>
#include "EventListener.h"
#include "Vec2.h"
#include "File.h"
#include "GuiCommandHandler.h"
#include "Rect.h"

namespace Amju
{
typedef void (*CommandFunc)();

class GuiElement : public EventListener
{
public:
  GuiElement();
  virtual ~GuiElement();
  virtual void Draw() = 0;
  virtual void Update() {} // ?
  virtual bool Load(File*); // Load pos and size
  bool OpenAndLoad(const std::string& filename);

  virtual GuiElement* GetElementByName(const std::string& name);

  // Selected items should look different to non-selected.
  void SetSelected(bool sel) { m_isSelected = sel; }
  bool IsSelected() const { return m_isSelected; }

  // Item with focus responds to keyboard events etc.
  void SetHasFocus(bool f);
  bool HasFocus() const;  

  virtual void OnGetFocus() {}
  void SetOnFocusFunc(CommandFunc onFocusFunc);

  // Set command as an object for Undo, or a function for simple, 
  //  non-undoable action
  void SetCommand(PGuiCommand pCommand);
  void SetCommand(CommandFunc commandFunc);
  void ExecuteCommand();

  void SetLocalPos(const Vec2f&);
  Vec2f GetLocalPos() const;
  Vec2f GetCombinedPos() const;

  void SetSize(const Vec2f&);
  Vec2f GetSize() const;

  // Scale factor so entire GUI can be zoomed in for accessibility
  static void SetGlobalScale(float f);
  static float GetGlobalScale();

  // Text to speech: if enabled, text-based elements speak when they 
  //  get focus. Also they should have a "speak" button.
  static void SetTextToSpeechEnabled(bool);
  static bool IsTextToSpeechEnabled();

  void SetVisible(bool isVis);
  bool IsVisible() const;

  void SetName(const std::string& name);
  const std::string& GetName() const;

  void SetParent(GuiElement* parent) { m_parent = parent; }
  GuiElement* GetParent() { return m_parent; }

  // For elements containing text, say the text.
  virtual void TextToSpeech() {}

protected:
  // Pos is top-left of element
  // Screen is (-1, -1)..(1, 1)
  /*
      (-1, 1) +--------------+ (1, 1)
              |              |
              |              |
              |              |
     (-1, -1) +--------------+ (1, -1)
  */
  Vec2f m_localpos;
//? Or calc on fly? //  Vec2f m_combinedPos;

  Vec2f m_size;
  std::string m_name;
  bool m_isVisible; // TODO Use flags if more 
  bool m_isSelected;

  // Only one of these is activated
  PGuiCommand m_pCommand;
  CommandFunc m_commandFunc;

  GuiElement* m_parent; // parent element, may be 0

  CommandFunc m_onFocusFunc; // called when this element gets the focus
};

typedef RCPtr<GuiElement> PGuiElement;
typedef std::vector<PGuiElement> GuiElements;

PGuiElement LoadGui(const std::string& filename, bool addAsListener = true);

// Convenience function - get rectangle from pos and size of element
Rect GetRect(GuiElement*);

// Prefer this function, as it can alert when no such element exists
GuiElement* GetElementByName(GuiElement* root, const std::string& nodeName);
}

#endif
