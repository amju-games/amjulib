#include <AmjuGL.h>
#include <Timer.h>
#include <AmjuRand.h>
#include "TitlePerson.h"
#include "LayerGroup.h"
#include "ROConfig.h"

namespace Amju
{
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
  static const float VEL_MIN = ROConfig()->GetFloat("title-vel-min", 0.3f);
  static const float VEL_MAX = ROConfig()->GetFloat("title-vel-max", 1.0f);

  m_xpos = Rnd(OFFSCREEN_MIN, OFFSCREEN_MAX);
  m_xvel = -Rnd(VEL_MIN, VEL_MAX); 

  float celltime = -0.25f / m_xvel;  
  // e.g. 1 unit/sec -> 0.25f; 2 units/sec -> 0.125

  m_spritenode.GetSprite().SetCellTime(celltime);

  if (rand() & 1)
  {
    m_xpos = -m_xpos;
    m_xvel = -m_xvel;
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
  for (unsigned int i = 0; i < m_people.size(); i++)
  {
    m_people[i].Update();
  }
}

void TitlePeople::Draw()
{
  for (unsigned int i = 0; i < m_people.size(); i++)
  {
    m_people[i].Draw();
  }
}

TitlePeople::TitlePeople()
{
  m_people.resize(5);
}
}


