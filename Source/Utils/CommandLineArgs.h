// Amjulib - (c) Juliet Colman 2025
#pragma once

// Command line args - set by main function where possible,
//  then can be read by game-specific code later on.

#include <string>
#include <vector>

namespace Amju
{
class CommandLineArgs 
{
public:
  using Strings = std::vector<std::string>;

  CommandLineArgs() = default;
  CommandLineArgs(int argc, char** argv);

  const Strings& GetArgs() const;

  bool Contains(const std::string& arg) const;

private:
  Strings m_strings;
};

void SetCommandLineArgs(const CommandLineArgs& cla);

const CommandLineArgs& GetCommandLineArgs();
}

