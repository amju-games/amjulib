#ifndef GS_LOADLEVEL_H_INCLUDED
#define GS_LOADLEVEL_H_INCLUDED

#include <File.h>
#include <GuiElement.h>
#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSLoadLevel : public GSBase
{
  GSLoadLevel();
  friend class Singleton<GSLoadLevel>;

public:
  virtual void Update() override;
  virtual void DrawScene() override;
  virtual void DrawScene2d() override;
  virtual void CreateTweakMenu() override;
  virtual void OnActive() override;

  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;

  void SetLevelFile(const std::string& levelFile) { m_levelFilename = levelFile; }
  std::string GetLastLoadedLevel() const;

private:
  bool LoadOneObject();

private:
  PGuiElement m_gui;
  RCPtr<File> m_file;
  std::string m_levelFilename;
  int m_numLoaded;
  int m_numObjects;
  float m_maxBarX;
  bool m_loadedOk;
};
typedef Singleton<GSLoadLevel> TheGSLoadLevel;
} // namespace
#endif
