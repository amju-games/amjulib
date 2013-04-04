#include <AmjuGL.h>
#include <Timer.h>
#include <AmjuRand.h>
#include "TitlePerson.h"
#include "LayerGroup.h"

namespace Amju
{
static const float VEL_MIN = 0.4f;
static const float VEL_MAX = 1.4f;

static const float SCREEN_RIGHT = 1.2f;
static const float SCREEN_LEFT = -SCREEN_RIGHT;

static const float OFFSCREEN_MIN = 1.2f;
static const float OFFSCREEN_MAX = 2.2f;

TitlePerson::TitlePerson()
{
  Randomise();
}

void TitlePerson::Randomise()
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();

  for (int i = 0; i < lgm->GetNumLayers(); i++)
  {
    int r = rand() % lgm->GetNumTextures(i);
    m_spritenode.SetLayerTex(i, r);

    r = rand() % lgm->GetNumColours();
    m_spritenode.SetLayerColour(i, r);
  }

  // Scale
  Vec2f scale(Rnd(0.5f, 1.5f), Rnd(0.5f, 1.5f));
  m_spritenode.SetScale(scale);

  // Move to one side of screen
  if (rand() & 1)
  {
    m_xpos = Rnd(OFFSCREEN_MIN, OFFSCREEN_MAX);
    m_xvel = -Rnd(VEL_MIN, VEL_MAX); 
  }  
  else
  {
    m_xpos = -Rnd(OFFSCREEN_MIN, OFFSCREEN_MAX);
    m_xvel = Rnd(VEL_MIN, VEL_MAX); // TODO CONFIG
  }
}

void TitlePerson::Draw()
{
  AmjuGL::PushMatrix();
  // Scale for 'breathing' effect..?
  AmjuGL::RotateX(90.0f);
  //static float f = 0;
  //f += TheTimer::Instance()->GetDt();
  //float s = sin(f) * 0.001f;
  AmjuGL::Translate(m_xpos, 0, 0.2f);
  AmjuGL::Scale(0.01f, 1, 0.01f);
  //AmjuGL::Scale(1, 1, 1.0f + s);

  m_spritenode.Draw();
 
  AmjuGL::PopMatrix();
}

void TitlePerson::Update()
{
  // TODO Maybe stop walking
  m_spritenode.SetAnimRange(0, 4);

  float dt = TheTimer::Instance()->GetDt();
  m_xpos += m_xvel * dt;
  if (m_xpos < -OFFSCREEN_MAX || m_xpos > OFFSCREEN_MAX)
  {
    Randomise();
  }
  m_spritenode.Update();
}

void TitlePeople::Update()
{
  for (int i = 0; i < m_people.size(); i++)
  {
    m_people[i].Update();
  }
}

void TitlePeople::Draw()
{
  for (int i = 0; i < m_people.size(); i++)
  {
    m_people[i].Draw();
  }
}

TitlePeople::TitlePeople()
{
  m_people.resize(5);

/*
  const int NUM_PEOPLE = 5;
  m_people.reserve(NUM_PEOPLE);
  for (int i = 0; i < NUM_PEOPLE; i++)
  {
    m_people.push_back(TitlePerson());
  }
*/
}
}


