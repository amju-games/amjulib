#ifndef R_B_H
#define R_B_H

#include <string>
#include <RCPtr.h>

namespace Amju
{
class RB : public RefCounted
{
public:
  RB();
  virtual ~RB() {}
  virtual void Draw() = 0;
  virtual void Update() = 0;

  typedef std::string TypeName;
  virtual TypeName GetTypeName() const = 0;

  void SetId(int id) { m_id = id; }
  int GetId() const { return m_id; }

  bool IsResting() const;
  void SetResting(bool isResting);

protected:
  int m_id;
  // in resting contact?
  bool m_atRest;
};
}

#endif

