#ifndef GS_LETTERCANCELLATION_H_INCLUDED
#define GS_LETTERCANCELLATION_H_INCLUDED

#include <Singleton.h>
#include <TextureSequence.h>
#include "GSGui.h"

namespace Amju 
{
class LCListener : public EventListener
{
public:
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  
};

class GSLetterCancellation : public GSGui
{
  GSLetterCancellation();
  friend class Singleton<GSLetterCancellation>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

  bool OnCursorEvent(const CursorEvent&);
  bool OnMouseButtonEvent(const MouseButtonEvent&);

  // Call this with different filenames prior to activating, to change task details.
  // Test ID is one of the enums in CogTestResults.
  bool LoadConfig(int testId, const std::string& filename);

  // Call to re/start
  void StartTest();

  // Called from Done button or when timer expires. Send results, go to next state.
  void FinishedTest();

protected:
  Rect MakeRect(int i, int j);

protected:
  // We display a grid of letters, 6 rows * 52 letters. This is defined by the research design.
  //std::vector<GuiElement*> m_letters; // Too slow!

  // Can't use GUI Text objects for the letters, as there are so many this is too slow.
  // Use 6 rows of tris.
  TextureSequence m_ts;
  AmjuGL::Tris m_tris[6]; // 6 rows of 52 chars, set by research design

  RCPtr<EventListener> m_listener;

  std::string m_fontImgFilename;
  float m_top; 
  float m_left; 
  float m_vSpacing;
  float m_hSpacing;
  float m_fontSize;
  float m_sqSize; // size of selection square

  char m_letters[6][52]; // the letters

  // The task is to click on the "special" letters without clicking on any others.
  // This is timed, with 3 mins to find all the special letters.
  char m_specialLetter;
  int m_numSpecialLetter;
  // String consisting of all other characters which the grid will contain
  std::string m_noSpecial;

  AmjuGL::Tris m_blocks; // blocks over/under ? selected letters

  // True if letter under mouse cursor
  bool m_mouseOver;
  Rect m_mouseRect; // to draw rectangle around letter under mouse cursor 

  float m_timer;
  float m_maxTime; // 3 mins in Malec et al

  bool m_isPaused; // ?
  bool m_isFinished;

  int m_testId; // to tag results sent to server
  int m_correct;
  int m_incorrect;
};
typedef Singleton<GSLetterCancellation> TheGSLetterCancellation;
} // namespace
#endif
