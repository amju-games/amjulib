#ifndef TITLE_PERSON_H_INCLUDED
#define TITLE_PERSON_H_INCLUDED

#include <RCPtr.h>
#include "Ve1SpriteNode.h"

namespace Amju
{
class TitlePerson : public RefCounted
{
public:
  TitlePerson();

  void Draw();
  void Update();
  void Randomise();

private:
  Ve1SpriteNode m_spritenode;
  float m_xpos;
  float m_xvel;
};

class TitlePeople
{
public:
  TitlePeople();
  void Update();
  void Draw();

private:
  typedef std::vector<TitlePerson> PersonVec;
  PersonVec m_people; 
};
}

#endif

