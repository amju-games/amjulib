#ifndef GS_STORY_H_INCLUDED
#define GS_STORY_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
// Shows a piece of storyline. No time to be nice, just show some text.
class GSStory : public GSGui
{
  GSStory();
  friend class Singleton<GSStory>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

  void OnReadOk(); 

private:
  bool m_showedOk; // true if player has read current story fragment, and we can go on to the next
  int m_storyLineNum; // index of current story fragment we are displaying
};
typedef Singleton<GSStory> TheGSStory;
} // namespace
#endif
