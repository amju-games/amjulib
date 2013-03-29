#include <AmjuFirst.h>
#include "MyTextMaker.h"
#include <AmjuGL.h>
#include <Colour.h>
#include <Timer.h>
#include <AmjuFinal.h>

namespace Amju
{
class MyCharacterDecorator : public SceneNode
{
public:
  MyCharacterDecorator(const Colour& colour, float theta);
  virtual void BeforeDraw();
  virtual void AfterDraw();

private:
  Colour m_colour;
  float m_theta;
};

MyCharacterDecorator::MyCharacterDecorator(const Colour& colour, float theta) :
  m_colour(colour), m_theta(theta)
{
}

void MyCharacterDecorator::BeforeDraw()
{
  AmjuGL::PushMatrix();
  PushColour();
  MultColour(m_colour);
  float s = sin(m_theta);

  float dt = TheTimer::Instance()->GetDt();
  m_theta += dt;
  AmjuGL::Translate(0, s, 0);
}

void MyCharacterDecorator::AfterDraw()
{
  PopColour();
  AmjuGL::PopMatrix();
}

MyTextMaker::MyTextMaker()
{
  m_theta = 0;
  SetDataDir("font3d/");
}

float rnd()
{
  float f = ((float)rand() / (float)RAND_MAX); // * 0.75f + 0.25f;
  return f;
}

PSceneNode MyTextMaker::Decorate(PSceneNode p)
{
  static const int MAX_COLOUR = 4;
  Colour COLOURS[MAX_COLOUR] = 
  {
    Colour(1.0f, 0.0f, 0.375f, 1.0f),
    Colour(0.8f, 0.0f, 0.3f, 1.0f),
    Colour(1.0f, 0.0f, 0.375f, 1.0f),
    Colour(0.8f, 0.0f, 0.3f, 1.0f)
    //Colour(1.0f, 0.0f, 0.7f, 1.0f),
    //Colour(1.0f, 0.0f, 0.9f, 1.0f)
  };
  // Choose colour
  static int c = 0;
  m_colour = COLOURS[c];
  c++;
  if (c == MAX_COLOUR)
  {
    c = 0;
  }
  SceneNode* decorator = new MyCharacterDecorator(m_colour, m_theta);
  // Get next value for theta
  m_theta += 0.2f; 

  decorator->AddChild(p);
  decorator->UpdateBoundingVol();

  return decorator;
}
}
