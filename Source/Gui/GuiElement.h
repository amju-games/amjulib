// Amjulib - cross platform game engine
// (c) Copyright Jason Colman 2000-2017

#pragma once

#include <functional>
#include <vector>
#include <EventListener.h>
#include <File.h>
#include <GuiCommandHandler.h>
#include <Rect.h>
#include <Vec2.h>

namespace Amju
{
class GuiEdit;

// Command function type.
// Gui Elements which respond to user events can call this kind of
//  function, to implement a lightweight action.
// For undoable commands, or commands requiring more data, there is the 
//  more heavyweight GuiCommand class.
using CommandFunc = std::function<void(GuiElement*)>;

// * GuiElement *
// Base class for GUI classes. Subclasses represent specific kinds of
//  GUI widgets. 
// GUI elements are held in a tree structure. We are using the Composite
//  and Decorator Design Patterns for this.
class GuiElement : public EventListener
{
public:
  GuiElement();
  virtual ~GuiElement();

  virtual std::string GetTypeName() const = 0;

  virtual void Draw() = 0;
  virtual void Update() {} 
  virtual bool Load(File*); // Load pos and size
  virtual bool Save(File*);

  // Create a GuiElement editor object which lets us edit this node.
  virtual GuiEdit* CreateEditor();

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
  // Called when we should execute the command set from one of the above
  virtual void ExecuteCommand();

  void SetLocalPos(const Vec2f&);
  Vec2f GetLocalPos() const;
  Vec2f GetCombinedPos() const;

  void SetSize(const Vec2f&);
  Vec2f GetSize() const;

  // Calculate bounding rectangle
  virtual Rect CalcRect() const;

  // Scale factor so entire GUI can be zoomed in for accessibility
  static void SetGlobalScale(float f);
  static float GetGlobalScale();

  // Text to speech: if enabled, text-based elements speak when they 
  //  get focus. Also they should have a "speak" button.
  static void SetTextToSpeechEnabled(bool);
  static bool IsTextToSpeechEnabled();

  virtual void SetVisible(bool isVis);
  bool IsVisible() const;

  void SetName(const std::string& name);
  const std::string& GetName() const;

  void SetParent(GuiElement* parent) { m_parent = parent; }
  GuiElement* GetParent() { return m_parent; }

  // For elements containing text, say the text.
  virtual void TextToSpeech() {}

  void SetDrawBorder(bool draw) { m_drawBorder = draw; }
 
  // Set arbitrary data for use in client callbacks etc
  void SetUserData(void* data) { m_userData = data; }
  void* GetUserData() { return m_userData; }

  // Conceptually protected:
  virtual void SetAncestorsVisible(bool ancestorVis);

  // Animation: this animation value varies between 0..1, set by an animating parent.
  // Override this to change a property like colour, position, rotation, etc.
  // When overriding this function in a composite/decorator, call the base class version 
  //  to pass the animation value down the tree to all descendants.
  virtual void Animate(float animValue) {}

  // Reset animation time to zero. Composite/decorators must pass this along to children.
  virtual void ResetAnimation() {}

  // Called when animation loops. Composite/decorators must pass this along to children.
  virtual void OnResetAnimation() {}

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

  Vec2f m_size;
  std::string m_name;
  bool m_isVisible; // TODO Use flags if more 
  bool m_isSelected;

  // Only one of these is activated
  PGuiCommand m_pCommand;
  CommandFunc m_commandFunc;

  GuiElement* m_parent; // parent element, may be 0

  CommandFunc m_onFocusFunc; // called when this element gets the focus

  bool m_drawBorder;

  void* m_userData;

  // True if all ancestors up to root are visible
  bool m_ancestorsAreVisible;
};

// Convenience typedefs
typedef RCPtr<GuiElement> PGuiElement;
typedef std::vector<PGuiElement> GuiElements;

PGuiElement LoadGui(const std::string& filename, bool addAsListener = true);

// Convenience function - get rectangle from pos and size of element
Rect GetRect(GuiElement*);

// Recursively search a GUI tree to find a named element.
// Prefer this function over the member function, as it can alert when no 
//  such element exists.
GuiElement* GetElementByName(GuiElement* root, const std::string& nodeName);
}
