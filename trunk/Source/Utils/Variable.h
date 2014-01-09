#pragma once

#include <string>
#include <vector>
#include "RCPtr.h"

namespace Amju
{
// Variable is a union of different types, like ints, floats, strings, lists 
// etc. 
// Useful when you want to pass different data types through the same 
//  parameter list. E.g. database field; GUI value; Lua params

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
  };
  

  Variable();

  Variable(bool);
  Variable(int);
  Variable(float);
  Variable(const std::string&);
  Variable(const VariableVec& v);

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

  Variable& operator=(bool);
  Variable& operator=(int);
  Variable& operator=(float);
  Variable& operator=(const std::string&);
  Variable& operator=(const VariableVec& v);
  
  bool GetBool() const;
  int GetInt() const;
  float GetFloat() const;
  std::string GetString() const;
  std::vector<Variable> GetVector() const;

  // Get string representation of the Variable, whatever its type.
  std::string ToString() const;

private:
  std::string VecToString() const;

private:
  // This was a union, to save space. But for such a tiny saving, it doesn't seem worth it - 
  //  unions are restrictive and error prone. The string and vector members would have to
  //  be stored on the heap, making the rest of the Variable class much more complex.
  struct Data 
  {
    bool b;
    int i;
    float f;
    std::string pString;
    std::vector<Variable> pVec;
  };

  Data m_data;

  Type m_type;
};
}




