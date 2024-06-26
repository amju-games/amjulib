#include <AmjuFirst.h>
#include "Animated.h"
#include "Timer.h"
#include "AmjuGL.h"
#include "Sign.h"
#include "ResourceManager.h"
#include "ReportError.h"
#include "DrawAABB.h"
#include "Player.h"
#include <AmjuFinal.h>

namespace Amju
{
Animated::Animated()
{
  m_frame = 0;
  m_nextFrame = 1;
  m_t = 0;
  m_anim = 0;
}

void Animated::SetFromCharacterName(const std::string& chName)
{
  // Characters must be consistently named: chName.md2
  std::string file = "characters/" + chName + "/" + chName + ".md2";

//std::cout << "Loading character MD2: " << file << "\n";

  LoadMd2(file);
}

void Animated::SetAnim(Animated::Anim anim)
{
  if (!m_pModel)
  {
//std::cout << "Warning: Animated: anim set but MD2 model not set yet\n";
    return;
  }

  // These names match order of Anim enum. The strings are the same in the MD2 model.
  // We allow MD2s to have anims in any order, so we look up per-model.
  static const int NUM_ANIMS = 3;
  static const char* ANIM_NAMES[NUM_ANIMS] = 
  {
    "stand",
    "walk",
    "walk" // because no run, TODO
  };
  if (anim >= NUM_ANIMS)
  {
std::cout << "WARNING Bad anim: " << (int)anim << " for Animated!!\n";
    return;
  }
  if (m_pModel->IsValidName(ANIM_NAMES[anim]))
  {
    int animCode = m_pModel->GetAnimationFromName(ANIM_NAMES[anim]);
    SetAnim(animCode);
  }
  else
  {
std::cout << "WARNING Invalid anim name: " << ANIM_NAMES[anim] << " for Animated!\n";
  }
}

void Animated::SetAnim(int anim)
{
  if (!m_pModel)
  {
//std::cout << "Warning: Animated: anim set but MD2 model not set yet\n";
    return;
  }

  Assert(anim != -1);

  if (anim == m_anim)
  {
    return;
  }

  m_anim = anim;
  // Blend into first frame of new anim
  m_nextFrame = m_pModel->GetStartFrame(m_anim);
}

Md2Model* Animated::GetMd2()
{
  return m_pModel;
}

void Animated::SetMd2(Md2Model* model)
{
  m_pModel = model;

  // Look out! Old frame values won't be valid anymore!
  if (m_pModel->GetAnimationSize(m_anim) == 0)
  {
    // Current anim doesn't exist in new model, reset
    m_frame = 0;
    m_nextFrame = 1;
    m_t = 0;
    m_anim = 0;
  }
  else
  {
    // Keep anim, but get new frame values
    m_frame = m_pModel->GetStartFrame(m_anim);
    m_nextFrame = m_frame + 1;
    m_t = 0;
  }
}

bool Animated::LoadMd2(const std::string& md2name)
{
  m_pModel = (Md2Model*)TheResourceManager::Instance()->GetRes(md2name);
  if (!m_pModel)
  {
    ReportError("Failed to load MD2: " + md2name);
    return false;
  }
  return true;
}

void Animated::Draw()
{
  Assert(IsVisible());

  if (!m_pModel)
  {
//std::cout << "Warning: Animated: can't draw, no MD2 model set yet.\n";
    return;
  }

  Assert(m_pModel);

  float t = m_t * 10.0f;
  if (t > 1.0f)
  {
    t = 1.0f;
  }
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  // TODO Offset Y so feet are at zero
  AmjuGL::Translate(0, 23.0f, 0);
  m_pModel->DrawFrames(m_frame, m_nextFrame, t);
  AmjuGL::PopMatrix();
}

void Animated::Update()
{
  if (!m_pModel)
  {
//std::cout << "Warning: Animated: can't update, no MD2 model set yet.\n";
    return;
  }

  Ve1Character::Update();
  
  if (!IsVisible())
  {
    return;
  }

  Player* player = dynamic_cast<Player*>(m_obj.GetPtr());
  if (player && !player->IsLoggedIn())
  {
    return;
  }

  Assert(m_pModel);
  float dt = TheTimer::Instance()->GetDt();
  m_t += dt;

  if (m_t > 0.1f) // MD2 frames are 0.1 secs each 
  {
    m_t = 0;
    m_frame = m_nextFrame;

    int startFrame = m_pModel->GetStartFrame(m_anim);
    int size = m_pModel->GetAnimationSize((Md2Model::Animation)m_anim);

    // If next frame is in the same animation, increment to next frame,
    //  or go back to first frame
    if (m_nextFrame >= startFrame && m_nextFrame <= startFrame + size)
    {
      m_nextFrame++;
      if (m_nextFrame >= startFrame + size)
      {
        if (m_pModel->GetDoesActionRepeat(m_anim))
        {
          m_nextFrame = startFrame;
          //m_gameObj->OnAnimRepeat();
        }
        else if (m_pModel->GetDoesActionFreeze(m_anim))
        {
          m_nextFrame--;
          //m_gameObj->OnAnimFreeze();
        }
        else
        {
          // TODO This doesn't work, overwritten by other code
          // Doesn't repeat or freeze - go back to stand
          m_anim = 0;
          m_nextFrame = 0;
          //m_gameObj->OnAnimFinished();
        }
      }
    }
  }
}
}
