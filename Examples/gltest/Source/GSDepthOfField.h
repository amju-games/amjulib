#pragma once

#include <Singleton.h>
#include <TriList.h>
#include <DepthOfField.h>
#include "GSBase.h"

namespace Amju 
{
class GSDepthOfField : public GSBase
{
  GSDepthOfField();
  friend class Singleton<GSDepthOfField>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

private:
  RCPtr<DepthOfField> m_dof;
};
typedef Singleton<GSDepthOfField> TheGSDepthOfField;
} // namespace

