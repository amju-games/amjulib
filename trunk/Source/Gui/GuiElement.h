#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <vector>
#include "RCPtr.h"
#include "EventListener.h"
#include "Vec2.h"
#include "File.h"
#include "GuiCommandHandler.h"
#include "Rect.h"

namespace Amju
{
class GuiElement : public RefCounted, public EventListener
{
public:
  virtual ~GuiElement();
  virtual void Draw() = 0;
  virtual void Update() {} // ?
  virtual bool Load(File*); // Load pos and size
  bool OpenAndLoad(const std::string& filename);

  virtual GuiElement* GetElementByName(const std::string& name);

  void SetCommand(PGuiCommand pCommand);
  void ExecuteCommand();

  void SetPos(const Vec2f&);
  const Vec2f& GetPos() const;

  void SetSize(const Vec2f&);
  const Vec2f& GetSize() const;

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
  Vec2f m_pos;
  Vec2f m_size;
  std::string m_name;

  PGuiCommand m_pCommand;
};

typedef RCPtr<GuiElement> PGuiElement;
typedef std::vector<PGuiElement> GuiElements;

PGuiElement LoadGui(const std::string& filename);

// Convenience function - get rectangle from pos and size of element
Rect GetRect(GuiElement*);
}

#endif
