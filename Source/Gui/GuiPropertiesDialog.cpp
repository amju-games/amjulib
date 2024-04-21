#include <GuiTextEdit.h>
#include <GuiButton.h>
#include "GuiPropertiesDialog.h"

namespace Amju
{
// One atomic list of all changes in one command
// This is a good idea, but we can generalise it surely. It's just a vec of Commands.
class PropertyChangeCommand : public GuiCommand
{
  // TODO
};

void GuiPropertiesDialog::TextItem::SetValue(const std::string& value)
{
  // Called when browse dialog OK'ed 
  m_value = value;
  // Update GUI text
  // TODO - not required?????
}

void GuiPropertiesDialog::TextItem::AddChangeToCommand(PropertyChangeCommand* pcc) 
{
  m_value = m_edit->GetText();
  //pcc->AddChange(PropertyChangeCommand::Change(m_key, m_value));
}

GuiComposite* GuiPropertiesDialog::TextItem::CreateGui() 
{
  // Create GUI elements
  GuiComposite* comp = new GuiComposite;
  GuiText* label = new GuiText;
  label->SetName("label");
  label->SetFgCol(Colour(1, 1, 1, 1)); // ?
  label->SetFontSize(1.0f);
  label->SetLocalPos(Vec2f(0, -0.05f)); // TODO
  label->SetSize(Vec2f(0.5f, 0.1f));
  label->SetJust(GuiText::AMJU_JUST_LEFT);
  label->SetText(m_label);
  comp->AddChild(label);

  m_edit = new GuiTextEdit;
  m_edit->SetName("value");
  m_edit->SetFgCol(Colour(1, 1, 1, 1)); // ?
  m_edit->SetBgCol(Colour(0, 0, 0, 1)); // ?
  m_edit->SetFontSize(1.0f);
  m_edit->SetLocalPos(Vec2f(0.5f, -0.05f)); // TODO
  m_edit->SetSize(Vec2f(0.5f, 0.1f));
  m_edit->SetText(m_value);
  comp->AddChild(m_edit);

  m_gui = comp;

  return m_gui;
}

GuiPropertiesDialog::IntItem::IntItem(const std::string& label, int value) : 
  TextItem(label, ToString(value))
{
}

void GuiPropertiesDialog::IntItem::AddChangeToCommand(PropertyChangeCommand* pcc) 
{
  m_value = m_edit->GetText();
//  pcc->AddChange(PropertyChangeCommand::Change(m_key, ToInt(m_value)));
}


static std::string s_lastPath; // TODO

// Special file dialog that knows the properties item to which it relates
class ItemFileDialog : public FileDialog
{
public:
  ItemFileDialog() : m_filenameItem(0) {}
  void SetItem(GuiPropertiesDialog::FilenameItem* filenameItem) { m_filenameItem = filenameItem; }
  GuiPropertiesDialog::FilenameItem* GetItem() { return m_filenameItem; }

private:
  GuiPropertiesDialog::FilenameItem* m_filenameItem;
};

static void OnBrowseDialogClosed(Dialog* dlg)
{
  if (dlg->GetResult() == AMJU_OK)
  {
    ItemFileDialog* ifd = dynamic_cast<ItemFileDialog*>(dlg);
    Assert(ifd);
    // Now call back with change to filename?
    ifd->GetItem()->SetValue(ifd->GetFilePath());
  }
}

class BrowseCommand : public GuiCommand
{
public:
  BrowseCommand(GuiPropertiesDialog::FilenameItem* filenameItem) : m_filenameItem(filenameItem)
  {
  }

  virtual bool Do() override
  {
    static ItemFileDialog s_fileDialog; 

    s_fileDialog.SetItem(m_filenameItem);
    s_fileDialog.SetGuiFilename("gui-file-load-preview-dialog.txt");
    s_fileDialog.SetFinishCallback(OnBrowseDialogClosed);
    s_fileDialog.SetPath("c:\\");
    s_fileDialog.SetTitle("Open File");
    DoModalDialog(&s_fileDialog);

    return false; // not undoable
  };

private:
  GuiPropertiesDialog::FilenameItem* m_filenameItem;
};

GuiComposite* GuiPropertiesDialog::FilenameItem::CreateGui() 
{
  TextItem::CreateGui();

  GuiButton* browseButton = new GuiButton;
  Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes("gui-button-4.png");
  // TODO Set font, size 
  browseButton->SetTexture(tex);
  browseButton->SetLocalPos(Vec2f(1.05f, -0.05f));
  browseButton->SetSize(Vec2f(0.4f, 0.1f));
  browseButton->SetText("Browse...");
  browseButton->SetCommand(new BrowseCommand(this));

  m_gui->AddChild(browseButton);

  return m_gui;
}

GuiPropertiesDialog::GuiPropertiesDialog()
{
  m_guiFilename = "gui-properties.txt";
}

void GuiPropertiesDialog::GetDataFromGui() 
{
}

void GuiPropertiesDialog::SetDataToGui()
{
}

void GuiPropertiesDialog::AddItem(PItem item)
{
  m_items.push_back(item);
}

void GuiPropertiesDialog::Clear()
{
  m_items.clear();
}

void GuiPropertiesDialog::OnOk()
{
  //RCPtr<PropertyChangeCommand> pcc = new PropertyChangeCommand(m_obj);

  //for (auto it = m_items.begin(); it != m_items.end(); ++it)
  //{
  //  PItem item = *it;
  //  item->AddChangeToCommand(pcc); // can skip if unchanged
  //}

  //// Skip if no changes
  //if (pcc->HasChanges())
  //{
  //  TheGuiCommandHandler::Instance()->DoNewCommand(pcc.GetPtr());
  //}
}

void GuiPropertiesDialog::OnActive()
{
  Dialog::OnActive();

  GuiComposite* parent = dynamic_cast<GuiComposite*>(m_gui.GetPtr());
  Assert(parent);

  float y = 0;
  for (auto it = m_items.begin(); it != m_items.end(); ++it)
  {
    PItem item = *it;

    GuiComposite* elem = item->CreateGui();
    elem->SetLocalPos(Vec2f(0, y));
    // Calc Size so we can offset each item vertically 
    elem->SetSizeFromChildren();
    y -= elem->GetSize().y;
    y -= 0.02f; // extra spacing - TODO

    parent->AddChild(elem);
  }
}

}

