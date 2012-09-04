#if !defined(OBJECTIVE_H_INCLUDED)
#define OBJECTIVE_H_INCLUDED

#include "Ve1Object.h"
#include <vector>

namespace Amju
{
class File;

class Objective //: public Ve1Object
{
public:

  // Load an objective from a file.
  bool Load(File* pf);

  // Returns true when the Objective has been met.
  // The list of Game Object IDs may refer to a game object that does
  // not exist. In this case you can fail immediately, or ignore that
  // game object ID.
  bool IsSatisfied(bool nonExistentObjectsFail) const;

  // Replace IDs in the Objective; used when an object ID is not known
  // until run time.
  void ReplaceId(int replaceThis, int withThis);

  enum Operator
  {
    AND,
    OR
  };


  bool Save(File*);

  Operator GetOp() const { return m_op; }
  void SetOp(Operator op) { m_op = op; }

private:
  Operator m_op;

  struct ObjectiveTriple
  {
    ObjectiveTriple() : m_id(-1) {}
    ObjectiveTriple(int id, const std::string& key, const std::string& val) : m_id(id), m_key(key), m_val(val) {}

    int m_id;
    std::string m_key;
    std::string m_val;
  };
  typedef std::vector<ObjectiveTriple> Triples;
  Triples m_triples;
};
}

#endif

