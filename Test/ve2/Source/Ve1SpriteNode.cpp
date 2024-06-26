#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <ReportError.h>
#include <Timer.h>
#include <AmjuRand.h>
#include "Ve1SpriteNode.h"
#include "LayerGroup.h"
#include "ROConfig.h"
#include "Player.h"
#include "GameMode.h"
#include <AmjuFinal.h>

namespace Amju
{
// Size of player sprites
static const float SIZE = 40.0f;
static const float CELL_TIME = 0.2f;

Ve1SpriteNode::Ve1SpriteNode()
{
  SetBlended(true);

  m_blinkTime = 0;
  m_maxBlinkTime = 0;
  m_gazeTime = 0;

  m_scale = Vec2f(1.0f, 1.0f);

  // This loads the base spritesheet, which we don't actually draw???
  // (TODO Change LayerSprite so we don't need it)
  std::string tex = "characters/2d/spritesheet.png";
  if (!m_sprite.Load(tex, 8, 1, SIZE * 0.5f, SIZE))
  {
    ReportError("FAILED TO LOAD sprite from char name");
    Assert(0);
  }

  m_sprite.SetCellTime(CELL_TIME);
  m_sprite.SetCellRange(1, 1);
  m_sprite.SetCell(1);

  m_eyes.Load(tex, 8, 1, SIZE * 0.5f, SIZE);
  m_eyes.SetCellTime(CELL_TIME);
  m_eyes.SetCellRange(1, 1);
  m_eyes.SetCell(1);

  // Add layers
  // TODO TEMP TEST -- really add them when we get state msgs
  // OR this is OK, the layers are set when we get server msgs. 
  // These are the default settings for each layer.
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();
  int numLayers = lgm->GetNumLayers();
  for (int i = 0; i < numLayers; i++)
  {
    // Fof layer i, set tex 0 and colour 0
    m_sprite.AddLayer(SpriteLayer(i, lgm->GetTexture(i, 0), lgm->GetColour(0), 0, 0));  
  }

  // TODO Add layers to m_eyes
  static std::pair<const char*, Colour> EYE_TEXTURE_COLOUR[] = 
  {
    std::make_pair("characters/2d/eyes-blink.png", Colour(1, 1, 1, 1)),
    std::make_pair("characters/2d/eyes_white.png", Colour(1, 1, 1, 1)),
    std::make_pair("characters/2d/eyes_look_se.png", Colour(0, 0, 1, 1))
  };

  for (int i = 0; i < 3; i++)
  {
    Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes(EYE_TEXTURE_COLOUR[i].first);
    Assert(tex);
    m_eyes.AddLayer(SpriteLayer(i, tex, EYE_TEXTURE_COLOUR[i].second, 0, 0));
  }
}

LayerSprite& Ve1SpriteNode::GetSprite() 
{
  return m_sprite;
}

void Ve1SpriteNode::SetLayerTex(int layer, int texIndex)
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();
  if (layer >= lgm->GetNumLayers())
  {
    return;
  }

  m_sprite.SetLayerTex(layer, texIndex);
}

void Ve1SpriteNode::SetLayerColour(int layer, int colIndex)
{
  static LayerGroupManager* lgm = TheLayerGroupManager::Instance();
  if (layer >= lgm->GetNumLayers())
  {
    return;
  }

  m_sprite.SetLayerColour(layer, colIndex);
}

void Ve1SpriteNode::SetFromCharacterName(const std::string& s)
{
}

void Ve1SpriteNode::SetScale(const Vec2f& scale)
{
  m_scale = scale;
}

const Vec2f& Ve1SpriteNode::GetScale() const
{
  return m_scale;
}

void Ve1SpriteNode::Draw()
{
  if (GetGameMode() == AMJU_MODE_SINGLE)
  {
    // Don't draw non-local player
    Player* p = dynamic_cast<Player*>(m_obj.GetPtr());
    if (p && !p->IsLocalPlayer())
    {
      return;
    }
  }

  AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  AmjuGL::RotateX(-90.0f); // so x-y plane is x-z plane

  AmjuGL::Scale(m_scale.x, m_scale.y, 1.0f);
  // Translate so centred on sprite
  AmjuGL::Translate(-SIZE * 0.5f, -SIZE, 0); 

  m_sprite.DrawLayers(Vec2f(0, 0), 2.0f);
  m_eyes.DrawLayers(Vec2f(0, 0), 2.0f);

  AmjuGL::PopMatrix();
  AmjuGL::PopAttrib();
}

void Ve1SpriteNode::Update()
{
  // Updates cell 
  m_sprite.Update();
  m_eyes.Update();

  Player* player = dynamic_cast<Player*>(m_obj.GetPtr());
  if (player && !player->IsLoggedIn())
  {
    // Not logged in - show closed eyes
    m_eyes.SetEyesOpen(false);
  }
  else
  {
    // TODO Don't blink if "asleep", i.e. logged out
    float dt = TheTimer::Instance()->GetDt();
    m_blinkTime += dt;
    if (m_blinkTime > m_maxBlinkTime)
    {
      m_eyes.SetEyesOpen(false);
      if (m_blinkTime > m_maxBlinkTime + 0.2f) // TODO
      {
        m_eyes.SetEyesOpen(true);
        m_blinkTime = 0;
        m_maxBlinkTime = Rnd(0.5f, 2.5f); // TODO
      }
    }

    // Set eye gaze dir
    static const int NUM_EYE_GAZE_TEXTURES = 2;
    static const char* EYE_GAZE[NUM_EYE_GAZE_TEXTURES] = 
    {
      "characters/2d/eyes_look_se.png",
      "characters/2d/eyes_look_sw.png",

    };
    // Get direction of interest. Randomize a bit?
    // Convert direction into index into EYE_GAZE.
    m_gazeTime += dt;
    static const float MAX_GAZE_TIME = ROConfig()->GetFloat("gaze-time", 1.0f);

    if (m_gazeTime > MAX_GAZE_TIME)
    {
      m_gazeTime = 0;
      int i = rand() % NUM_EYE_GAZE_TEXTURES;
      Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes(EYE_GAZE[i]);
      m_eyes.SetEyeGazeTex(tex);
    }
  }
}

void Ve1SpriteNode::SetAnimRange(int a, int b)
{
  int c = m_sprite.GetCell();
  m_sprite.SetCellRange(a, b);
  m_eyes.SetCellRange(a, b);
  if (c < a || c > b)
  {
    m_sprite.SetCell(a);
    m_eyes.SetCell(a);
  }
}

void Ve1SpriteNode::SetAnim(Ve1SpriteNode::Anim anim)
{
  // ?
/*
  if (anim == m_currentAnim)
  {
    return;
  }
*/

  // TODO Set anim range here
  // BUT also based on direction
  if (anim == ANIM_IDLE)
  {
    int c = 1; 
    SetAnimRange(c, c);
  }
  else
  {
    Vec3f dir = m_obj->GetVel();
    // 2 directional: 
    // Up or down ?
    if (dir.z < 0)
    {
      // Up?
      SetAnimRange(4, 7);
    }
    else
    {
      SetAnimRange(0, 3);
    }

#ifdef FOUR_DIRECTIONAL
    if (fabs(dir.x) > fabs(dir.z))
    {
      // Left or right
      if (dir.x < 0)
      {
        // Left
        SetAnimRange(12, 15);
      }
      else
      {
        // Right
        SetAnimRange(8, 11);
      }
    }
    else
    {
      // Up or down
      if (dir.z < 0)
      {
        // Up?
        SetAnimRange(4, 7);
      }
      else
      {
        SetAnimRange(0, 3);
      }
    }
#endif

  }
}
}

