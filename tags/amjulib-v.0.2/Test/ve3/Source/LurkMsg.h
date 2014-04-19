#ifndef LURK_MSG_H_INCLUDED
#define LURK_MSG_H_INCLUDED

#include <map>
#include <queue>
#include <string>
#include <Colour.h>
#include <GuiText.h>
#include <GuiRect.h>
#include <GuiButton.h>
#include <Singleton.h>
#include <StringUtils.h>

namespace Amju
{
enum LurkPos { AMJU_NONE, AMJU_TOP, AMJU_BOTTOM, AMJU_LEFT, AMJU_RIGHT, AMJU_CENTRE };

// Lurk on edge of screen
class LurkMsg
{
public:
  LurkMsg();
  // Optional function which is called when message has been displayed and has now gone away
  LurkMsg(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp, 
    CommandFunc onFinished = 0);

  void Draw();
  void Update();
  void Set(const std::string& text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
    CommandFunc onFinished = 0);

  void Set(GuiText* text, const Colour& fgCol, const Colour& bgCol, LurkPos lp,
    CommandFunc onFinished = 0);

  bool IsFinished() const;

  void SetOkCommand(CommandFunc onOk);
  void SetYesCommand(CommandFunc onYes);
  void SetNoCommand(CommandFunc onNo);
 
  void DoOk();
  void DoYes();
  void DoNo();

private:
  RCPtr<GuiText> m_text;
  RCPtr<GuiRect> m_rect;

  Vec2f m_pos; // current pos
  Vec2f m_showPos; // position when LURK_SHOWN state is reached
  Vec2f m_hidePos; // position when fully hidden
  Vec2f m_vel;
  float m_scale; // for centre msgs which scale up and down

  float m_timer;
  
  enum State { LURK_NEW, LURK_SHOWN, LURK_SHOWING, LURK_HIDDEN, LURK_HIDING, LURK_FINISHED };
  State m_state;

  friend class Lurker;
  LurkPos m_lurkPos;

  CommandFunc m_onOk;
  CommandFunc m_onYes;
  CommandFunc m_onNo;
};

class Lurker : public NonCopyable
{
  Lurker();
  friend class Singleton<Lurker>;

public:
  void Update();
  void Draw();
  void Queue(const LurkMsg& lm);

  // TODO
  void QueueMultiPage(const Strings& strs, const Colour& fgCol, const Colour& bgCol);

  void ShowYesNo(const std::string& q, const Colour& fgCol, const Colour& bgCol, 
    CommandFunc no, CommandFunc yes);

  void Clear(); // to prevent old msgs from stacking up

  void OnLurkOk();
  void OnLurkYes();
  void OnLurkNo();

  void TextToSpeech(const std::string& text);

  bool IsDisplayingMsg() const;

private:
  typedef std::queue<LurkMsg> LurkMsgQ; // queue of msgs or one Lurk pos
  typedef std::map<LurkPos, LurkMsgQ> QMap; 
  QMap m_qmap; // one queue for each position

  // next/ok button for Centred text (TODO make this per-LurkMsg ?)
  //RCPtr<GuiButton> m_button; 
  PGuiElement m_gui; // yes/no/ok buttons
  GuiButton* m_ok;
  GuiButton* m_yes;
  GuiButton* m_no;
};

typedef Singleton<Lurker> TheLurker;
}

#endif

