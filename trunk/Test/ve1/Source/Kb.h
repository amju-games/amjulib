#ifndef KB_H_INCLUDED
#define KB_H_INCLUDED

#include <GuiKeyboard.h>
#include <Singleton.h>
#include <StringUtils.h>

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

  // Set a list of layout filenames
  void SetPages(const Strings& pageFilenames);

  // Call to change page
  bool Load(const std::string& guiKbFilename);

  // Called when Next/Prev page button pressed
  void OnNextPage();
  void OnPrevPage();

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

  // Filenames for a sequence of layouts
  Strings m_pages;
  int m_currentPage;
};

typedef Singleton<Kb> TheKb;

// For convenience, using hardcoded filenames to hardcode kb layouts!
// (But can still be localised by having a localised file name in each gui-kb-* file)
enum KbLayout
{
  KB_LAYOUT_REGULAR,
  KB_LAYOUT_EMAIL
};

void KbSetLayout(KbLayout);
}

#endif

