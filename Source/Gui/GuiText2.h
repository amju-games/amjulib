#pragma once

#include "Batched.h"
#include "GuiText.h"

namespace Amju
{
// ** GuiText2 **
// Simple text class without all the baggage of GuiText.
// Best choice for fast, batched text, i.e. this is what you
//  want 99.9% of the time.
class GuiText2 : public GuiTextBase, public Batched
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiText2* Clone() override { return new GuiText2(*this); }

  virtual void Draw() override;
  virtual bool Load(File*) override;

  Texture* GetTexture() override; // for Batched
  void AddToTrilist(AmjuGL::Tris& tris) override; // for Batched

  void SetText(const std::string& text) override; // IGuiText

protected:
  void BuildTriList();

  bool HandleAttrib(const std::string& s) override; 
  bool HandleAttrib(const std::string& key, const std::string& value) override; 
  
protected:
  AmjuGL::Tris m_tris;
  Strings m_strings;
};
}
 
