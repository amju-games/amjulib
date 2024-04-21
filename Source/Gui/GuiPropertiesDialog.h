#pragma once

#include "ModalDialog.h"

namespace Amju
{
class PropertyChangeCommand;
class GuiTextEdit;

// Types for properties
// TODO Why do we need this? Surely we just have subclasses and that's it??
//enum ItemType
//{
//  AMJU_PROP_TEXT,
//  AMJU_PROP_FILE,
//  AMJU_PROP_INT,
//  AMJU_PROP_FLOAT,
//  AMJU_PROP_BOOL,
//  AMJU_PROP_VEC3
//};

class GuiPropertiesDialog : public Dialog
{
public:
  // One item in the dialog
  struct Item : public RefCounted
  {
    Item(const std::string& label) : m_label(label)
    {
    }

    virtual GuiComposite* CreateGui() = 0;

    virtual void AddChangeToCommand(PropertyChangeCommand*) = 0;

    std::string m_label; // displayed in GUI
    ////ItemType m_itemType; // so we know how to display and change
    // Call back when changed?

    RCPtr<GuiComposite> m_gui;

    ////PropertyKey m_key; // Key to set the object member variable
  };
  typedef RCPtr<Item> PItem;
  typedef std::vector<PItem> Items;

  struct TextItem : public Item
  {
    TextItem(const std::string& label, const std::string& value) : Item(label),
      m_value(value), m_edit(0)
    {}

    virtual GuiComposite* CreateGui() override;
    virtual void AddChangeToCommand(PropertyChangeCommand*) override;

    void SetValue(const std::string& value);

  protected:
    std::string m_value;
    GuiTextEdit* m_edit; // pointer to edit control so we can read back changes to text value
  };

  struct FilenameItem : public TextItem
  {
    FilenameItem(const std::string& label, const std::string& value) : TextItem(label, value)
    {
      //m_itemType = AMJU_PROP_FILE;
    }

    virtual GuiComposite* CreateGui() override;
  };

  struct IntItem : public TextItem
  {
    IntItem(const std::string& label, int value);

    virtual void AddChangeToCommand(PropertyChangeCommand*) override;
  };

public:
  GuiPropertiesDialog();

  virtual void OnActive() override;
  virtual void GetDataFromGui() override;
  virtual void SetDataToGui() override;
  virtual void OnOk() override;

  //void SetObj(WWGameObject* obj);
  void Clear();
  void AddItem(PItem);

protected:
  Items m_items;
//  int m_numItems;
//  RCPtr<WWGameObject> m_obj;
};
}
