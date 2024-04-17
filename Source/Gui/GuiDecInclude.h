// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include "GuiDecorator.h"

namespace Amju
{
// * GuiDecInclude *
// Decorator which loads child from a file. This lets us reuse GUIs in files.
class GuiDecInclude : public GuiDecorator 
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  virtual bool Load(File*) override;
  virtual bool Save(File*) override;

  const std::string& GetErrorStr() const;

private:
  std::string m_includeFilename;
  std::string m_errorString; // location of include, for error reporting
};
}

