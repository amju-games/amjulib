#pragma once

#include <string>
#include <vector>
#include <Vec2.h>
#include <Vec3.h>
#include "RCPtr.h"

namespace Amju
{
// Variable is a union of different types, like ints, floats, strings, lists 
// etc. 
class Variable;
typedef std::vector<Variable> VariableVec;

class Variable : public RefCounted
{
public:
  enum Type 
  {
    VARIABLE_NULL,
    VARIABLE_BOOL,
    VARIABLE_INT,
    VARIABLE_FLOAT,
    VARIABLE_STRING,
    VARIABLE_VECTOR,
    VARIABLE_VEC2,
    VARIABLE_VEC3
  };
  

  Variable();
  Variable(const Variable& var);

  Variable(bool);
  Variable(int);
  Variable(float);
  Variable(const std::string&);
  Variable(const VariableVec& v);
  Variable(const Vec2f&);
  Variable(const Vec3f&);

  ~Variable();

  // Reset the Variable to Null.
  void Clear();

  Type GetType() const;

  bool IsNull() const;
  bool IsBoolType() const;
  bool IsIntType() const;
  bool IsFloatType() const;
  bool IsStringType() const;
  bool IsVectorType() const;
  bool IsVec2Type() const;
  bool IsVec3Type() const;

  Variable& operator=(const Variable& var);
  Variable& operator=(bool);
  Variable& operator=(int);
  Variable& operator=(float);
  Variable& operator=(const std::string&);
  Variable& operator=(const VariableVec& v);
  Variable& operator=(const Vec2f& v);
  Variable& operator=(const Vec3f& v);
  
  bool GetBool() const;
  int GetInt() const;
  float GetFloat() const;
  std::string GetString() const;
  std::vector<Variable> GetVector() const;
  Vec2f GetVec2() const;
  Vec3f GetVec3() const;

  // Get string representation of the Variable, whatever its type.
  std::string ToString() const;

private:
  void Swap(Variable* pOther);

  std::string VecToString() const;

private:
  union Data
  {
    bool b;
    int i;
    float f;
    // More interesting types have to be on the heap
    std::string* pString;
    std::vector<Variable>* pVec;
    Vec2f* vec2;
    Vec3f* vec3;
  };

  Data m_data;

  Type m_type;
};
}




