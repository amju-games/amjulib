#ifndef MY_TEXT_MAKER_H
#define MY_TEXT_MAKER_H

#include <TextMaker.h>
#include <Colour.h>

namespace Amju
{
class MyTextMaker : public TextMaker
{
public:
  MyTextMaker();
  virtual PSceneNode Decorate(PSceneNode p);

private:
  Colour m_colour;
  float m_theta;
};
}

#endif


