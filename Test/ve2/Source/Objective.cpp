#include <AmjuFirst.h>
#include <Game.h>
#include "Objective.h"
#include "File.h"
#include <AmjuFinal.h>

namespace Amju
{
bool Objective::Load(File* pf)
{
  AMJU_CALL_STACK;

  // The objective is represented by a list of <game object ID, state ID> 
  // pairs. A pair evaluates to true if the specifed game object has the
  // state specfied. If all pairs are true, the objective is satisfied.

  // First thing is a function. This tells us whether all the pairs must
  // be satisfied, or any of them, or what.
  std::string funcName;
  pf->GetDataLine(&funcName);
  if (funcName == "and")
  {
    m_op = AND;
  }
  else if (funcName == "or")
  {
    m_op = OR;
  }
  else
  {
    pf->ReportError("Expected operator, e.g. \"and\".");
    return false;
  }

  int numPairs = 0;
  if (!pf->GetInteger(&numPairs))
  {
    pf->ReportError("Expected number of objective pairs.");
    return false;
  }
  for (int i = 0; i < numPairs; i++)
  {
    // Each line is in form "<game object ID>, <key>, <val>"
    std::string str;
    if (!pf->GetDataLine(&str))
    {
      pf->ReportError("Expected objective game object/key/value triple");
      return false;
    }
    Strings strs = Split(str, ',');
    if (strs.size() != 3)
    {
      pf->ReportError("Unexpected format for objective game object/key/value triple");
      return false;
    }
    int gameObjId = ToInt(strs[0]);
    
    ObjectiveTriple tr(gameObjId, strs[1], strs[2]); 
    m_triples.push_back(tr);
  }

  return true;
}

bool Objective::Save(File* pf)
{
  AMJU_CALL_STACK;

  pf->WriteComment("// Objective: operator");
  if (m_op == AND)
  {
    pf->Write("and");
  }
  else if (m_op == OR)
  {
    pf->Write("or");
  }
  pf->WriteComment("// Objective: no of pairs, then (game id, state) pairs");
  pf->WriteInteger((int)m_triples.size());
  for (unsigned int i = 0; i < m_triples.size(); i++)
  {
    const ObjectiveTriple& tr = m_triples[i];
    std::string s = ToString(tr.m_id) + "," + tr.m_key + "," + tr.m_val;
    pf->Write(s);
  }
  return true;
}

bool Objective::IsSatisfied(bool nonExistentObjectsFail) const
{
  AMJU_CALL_STACK;

  // Iterate over the objective pairs, and test each one.
  for (Triples::const_iterator it = m_triples.begin();
       it != m_triples.end(); 
       ++it)
  {
    const ObjectiveTriple& tr = *it;
    int gameObjId = tr.m_id;

    GameObject* go = TheGame::Instance()->GetGameObject(gameObjId);
    if (!go)
    {
      // The specified game object does not exist.
      // We let the caller decide how to handle this.
      if (nonExistentObjectsFail)
      {
        return false;
      }
      else
      {
        continue;
      }
    }

    Ve1Object* v = dynamic_cast<Ve1Object*>(go);
    Assert(v); // can only handle Ve1Objects

    // Check key/val for this object 
    const std::string& key = tr.m_key;
    const std::string& val = tr.m_val;
    bool exprIsTrue = (v->Exists(key) && v->GetVal(key) == val); 
    if (m_op == AND && !exprIsTrue)
    {
      return false;
    }
    if (m_op == OR && exprIsTrue)
    {
      return true;
    }
  }
  return true;
}

void Objective::ReplaceId(int replaceThis, int withThis)
{
  AMJU_CALL_STACK;
  
  for (Triples::iterator it = m_triples.begin();
       it != m_triples.end(); 
       ++it)
  {
    ObjectiveTriple& tr = *it;
    if (tr.m_id == replaceThis)
    {
      tr.m_id = withThis;
    }
  }
}

}

