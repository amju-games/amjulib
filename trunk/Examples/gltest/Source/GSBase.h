#ifndef GS_BASE_H
#define GS_BASE_H

#include <string>
#include <Camera.h>
#include <GameState.h>
#include <GuiDialog.h>
#include <ModalDialog.h>
#include "BarrelDist.h"

namespace Amju
{
// Use this for render to texture etc
void DrawBunnyScene();

class ChooserDialog : public Dialog
{
public:
  virtual void GetDataFromGui() override {}
  virtual void SetDataToGui() override {}
  void Populate(); // add names of all states in statelist
  void Choose();

private:
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
  virtual void OnDeactive() override;

  virtual bool OnKeyEvent(const KeyEvent&) override;
  virtual bool OnRotationEvent(const RotationEvent&) override;

  // Override to set up tweak menu. Call base impl CreateTweakMenu in your overriden function first.
  virtual void CreateTweakMenu();

  // Do the drawing here, NOT in Draw(): this is so we can draw in stereo if required.
  virtual void DrawScene() = 0;
  virtual void DrawScene2d();

  // Public as called from callbacks 
  void OnTweakButton();
  void OnChooseButton();
  void OnChooseOk();
  void OnChooseCancel();

  void SetStereo(bool stereo);

protected:
  void DrawHelp(); // set matrices, probably will be using Camera instead

protected:
  float m_time;
  float m_maxTime;
  GSBase* m_nextState;
  std::string m_name;
  std::string m_description;
  GuiText m_guiText[2];
  bool m_paused;
  bool m_showTweak;
  bool m_showChoose;

  // Menu buttons
  RCPtr<GuiElement> m_menuButtons;

  // Tweak dialog
  RCPtr<GuiDialog> m_tweaker;

  // Choose state dialog
  RCPtr<GuiDialog> m_chooser;

  BarrelDist m_barrel;

  Camera m_camera;

  static bool s_isStereo;
  static bool s_enableBarrel;
  static bool s_mouseLook;
};
}

#endif
