#ifndef GS_BASE_H
#define GS_BASE_H

#include <string>
#include <GameState.h>
#include <GuiDialog.h>
#include <ModalDialog.h>

namespace Amju
{
// Use this for render to texture etc
void DrawBunnyScene();

class ChooserDialog : public Dialog
{
public:
  virtual void GetDataFromGui() override {}
  virtual void SetDataToGui() override {}

  void Add(const std::string& choice);
};

class GSBase : public GameState
{
public:
  GSBase();
  virtual void Update() override;
  virtual void Draw() override; // Set up proj and mview matrices
  virtual void Draw2d() override;
  virtual void OnActive() override;

  virtual bool OnKeyEvent(const KeyEvent&) override;

protected:
  float m_time;
  float m_maxTime;
  GSBase* m_nextState;
  std::string m_name;
  bool m_paused;

//  RCPtr<GuiDialog> m_dlg;
};
}

#endif
