#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// Decorator: moves child from an offset to the final intended position 
class GuiElastic : public GuiDecorator
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiElastic();
  virtual bool Load(File*);
  virtual bool Save(File*);
  virtual void Update();

  // Set the velocity at which we move from the offset to final position.
  // This applies to all elastics.
  static void SetElasticVel(float vel);
  
protected:
  Vec2f m_offsetPos; // localpos starts here, moves to (0, 0)
  Vec2f m_acc;
  Vec2f m_vel;
  Vec2f m_origVel;
  int m_bounceX, m_bounceY;
  static float s_elasticVel;
};
}

