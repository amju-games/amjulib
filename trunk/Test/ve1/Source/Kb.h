#ifndef KB_H_INCLUDED
#define KB_H_INCLUDED

#include <GuiKeyboard.h>
#include <Singleton.h>

namespace Amju
{
class Kb
{
private:
  Kb();
  friend class Singleton<Kb>;

public:
  void Activate();
  void Deactivate();
  void Update();
  void Draw();

  void SetEnabled(bool);
  bool IsEnabled() const;

  // Call to change page
  bool Load(const std::string& guiKbFilename);

private:
  RCPtr<GuiKeyboard> m_kb;

  enum Mode
  {
    KB_SHOWN,
    KB_HIDDEN,
    KB_SHOWING,
    KB_HIDING
  };
  Mode m_mode;

  bool m_enabled;
};

typedef Singleton<Kb> TheKb;
}

#endif

