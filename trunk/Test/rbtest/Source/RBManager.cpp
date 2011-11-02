#include "RBManager.h"
#include <iostream>

namespace Amju
{
void RBManager::AddRB(RB* rb)
{
  m_rbs.push_back(rb);
}

void RBManager::Clear()
{
  m_rbs.clear();
}

float RBManager::GetDt() const
{
  return 0.0002f; // TODO TEMP TEST
}

bool RBManager::AddRBFunc(RB::TypeName tn1, RB::TypeName tn2, RBFunc f)
{
  TNPair tnp(tn1, tn2);
  m_map[tnp] = f;
  return true;
}

bool RBManager::Dispatch(RB* rb1, RB* rb2)
{
  TNPair tnp(rb1->GetTypeName(), rb2->GetTypeName());
  RBMap::iterator it;
  it = m_map.find(tnp);
  if (it != m_map.end())
  {
    RBFunc f = it->second;
    f(rb1, rb2);
    return true;
  }
  std::swap(tnp.first, tnp.second);
  if (it != m_map.end())
  {
    RBFunc f = it->second;
    f(rb2, rb1);
    return true;
  }
  return false;
}

void RBManager::Update()
{
  for (RBs::iterator it = m_rbs.begin(); it != m_rbs.end(); ++it)
  {
    (*it)->Update();
  }

  // Collisions - TODO use SAP
  for (RBs::iterator it = m_rbs.begin(); it != m_rbs.end(); ++it)
  {
    RB* rb1 = *it;

    for (RBs::iterator jt = it + 1; jt != m_rbs.end(); ++jt)
    {
      RB* rb2 = *jt;

      // Both resting, so don't check/respond to collision between them
      if (rb1->IsResting() && rb2->IsResting())
      { 
        //std::cout << "Resting\n";
        continue;
      }
    
      // Use double dipatcher
      Dispatch(rb1, rb2);
    }
  }
}

void RBManager::Draw()
{
  for (RBs::iterator it = m_rbs.begin(); it != m_rbs.end(); ++it)
  {
    (*it)->Draw();
  }
}
}
