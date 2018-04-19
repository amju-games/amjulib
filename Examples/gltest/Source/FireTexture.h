#pragma once

#include <AmjuTypes.h>
#include <GameObject.h>
#include <Texture.h>
#include "WithSceneNode.h"

namespace Amju
{
const int FIRE_SIZE = 32;
const int FIRE_ARRAY_SIZE = FIRE_SIZE * FIRE_SIZE;

class FireTexture;

class Fire : public GameObject, public WithSceneNode
{
public:
  Fire();
  static const char* NAME;
  const char* GetTypeName() const override;
  bool Load(File*) override;
  void Update() override;
  
protected:
  SceneGraph* GetSceneGraph() override;

private:
  FireTexture* m_firetex;
};

class FireTexture : public GameObject
{
public:
  FireTexture();
  static const char* NAME; 
  const char* GetTypeName() const override;
  void Update() override;

  Texture* GetTexture() { return &m_tex; }

protected: 
  bool Init();

protected:
  Texture m_tex;
 
  uint8 src[FIRE_ARRAY_SIZE];
  uint8 dst[FIRE_ARRAY_SIZE];
};
}

