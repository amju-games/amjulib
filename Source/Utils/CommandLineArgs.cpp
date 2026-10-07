// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include
#include "CommandLineArgs.h"

namespace Amju
{
CommandLineArgs::CommandLineArgs(int argc, char** argv)
{
  for (int i = 0; i < argc; i++)
  {
    m_strings.push_back(argv[i]);
  }
}

const CommandLineArgs::Strings& CommandLineArgs::GetArgs() const 
{
  return m_strings;
}

bool CommandLineArgs::Contains(const std::string& arg) const
{
  return (find(m_strings.begin(), m_strings.end(), arg) != m_strings.end());
}

static CommandLineArgs theArgs;

void SetCommandLineArgs(const CommandLineArgs& args)
{
  theArgs = args;
}

const CommandLineArgs& GetCommandLineArgs()
{
  return theArgs;
}
}

