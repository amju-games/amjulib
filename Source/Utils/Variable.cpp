#include <AmjuFirst.h>
#include "Variable.h"
#include "StringUtils.h"
#include "AmjuAssert.h"
#include <AmjuFinal.h>

namespace Amju
{
Variable::Variable()
{
  AMJU_CALL_STACK;

  m_type = VARIABLE_NULL;
}

Variable::Variable(bool b)
{
  AMJU_CALL_STACK;

  m_type = VARIABLE_BOOL;
  m_data.b = b;
}

Variable::Variable(int i)
{
  AMJU_CALL_STACK;

  m_type = VARIABLE_INT;
  m_data.i = i;
}

Variable::Variable(float f)
{
  AMJU_CALL_STACK;

  m_type = VARIABLE_FLOAT;
  m_data.f = f;
}

Variable::Variable(const std::string& s)
{
  AMJU_CALL_STACK;

  m_type = VARIABLE_STRING;
  m_data.pString = s;
}

Variable::Variable(const VariableVec& v)
{
  AMJU_CALL_STACK;

  m_type = VARIABLE_VECTOR;
  m_data.pVec = v;
}

Variable::~Variable()
{
  AMJU_CALL_STACK;

  Clear();
}

Variable& Variable::operator=(bool b)
{
  AMJU_CALL_STACK;

  Clear();
  m_type = VARIABLE_BOOL;
  m_data.b = b;
  return *this;
}

Variable& Variable::operator=(int i)
{
  AMJU_CALL_STACK;

  Clear();
  m_type = VARIABLE_INT;
  m_data.i = i;
  return *this;
}

Variable& Variable::operator=(float f)
{
  AMJU_CALL_STACK;

  Clear();
  m_type = VARIABLE_FLOAT;
  m_data.f = f;
  return *this;
}

Variable& Variable::operator=(const std::string& s)
{
  AMJU_CALL_STACK;

  Clear();
  m_type = VARIABLE_STRING;
  m_data.pString = s;
  return *this;
}

Variable& Variable::operator=(const VariableVec& v)
{
  AMJU_CALL_STACK;

  Clear();
  m_type = VARIABLE_VECTOR;
  m_data.pVec = v;
  return *this;
}

void Variable::Clear()
{
  AMJU_CALL_STACK;

  m_type = VARIABLE_NULL;
}

Variable::Type Variable::GetType() const
{
  return m_type;
}

bool Variable::IsNull() const
{
  AMJU_CALL_STACK;

  return m_type == VARIABLE_NULL;
}

bool Variable::IsBoolType() const
{
  AMJU_CALL_STACK;

  return m_type == VARIABLE_BOOL;
}

bool Variable::IsIntType() const
{
  AMJU_CALL_STACK;

  return m_type == VARIABLE_INT;
}

bool Variable::IsFloatType() const
{
  AMJU_CALL_STACK;

  return m_type == VARIABLE_FLOAT;
}

bool Variable::IsStringType() const
{
  AMJU_CALL_STACK;

  return m_type == VARIABLE_STRING;
}

bool Variable::IsVectorType() const
{
  AMJU_CALL_STACK;

  return m_type == VARIABLE_VECTOR;
}

bool Variable::GetBool() const 
{
  AMJU_CALL_STACK;

  Assert(IsBoolType());
  return m_data.b;
}

int Variable::GetInt() const
{
  AMJU_CALL_STACK;

  Assert(IsIntType());
  return m_data.i;
}

float Variable::GetFloat() const
{
  AMJU_CALL_STACK;

  Assert(IsFloatType() || IsIntType());
  if (IsFloatType())
  {
    return m_data.f;
  }
  else
  {
    return (float)m_data.i;
  }
  return 0;
}

std::string Variable::GetString() const
{
  Assert(IsStringType());
  return m_data.pString;
}

std::vector<Variable> Variable::GetVector() const
{
  Assert(IsVectorType());
  return m_data.pVec;
}

std::string Variable::ToString() const
{
  switch (m_type)
  {
  case VARIABLE_NULL:
    return "<null>";

  case VARIABLE_BOOL:
    return m_data.b ? "true" : "false";

  case VARIABLE_INT:
    return Amju::ToString(m_data.i);

  case VARIABLE_FLOAT:
    return Amju::ToString(m_data.f);

  case VARIABLE_STRING:
    return "\"" + m_data.pString + "\"";

  case VARIABLE_VECTOR:
    return VecToString();
  }

  Assert(0);
  return "<unknown type>";
}

std::string Variable::VecToString() const
{
  Assert(IsVectorType());

  if (m_data.pVec.empty())
  {
    return "<empty list>";
  }

  const int size = m_data.pVec.size();

  std::string res = "list [";
  res += Amju::ToString(size);
  res += "]: (";

  for (int i = 0; i < (int)size; i++)
  {
    res += m_data.pVec[i].ToString();
    if (i < (int)size - 1)
    {
      res += ", ";
    }
  }
  res += ")";
  return res;
}

}
