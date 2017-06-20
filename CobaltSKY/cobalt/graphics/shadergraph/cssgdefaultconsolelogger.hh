#pragma once

#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>


class CSE_API csSGDefaultConsoleLogger : public iSGShaderGraphLogger
{
public:
  csSGDefaultConsoleLogger();
  virtual ~csSGDefaultConsoleLogger(); 

  virtual void LogSourceCode(const csString &renderPass, const csString &shaderType, const csString &sourceCode);
  virtual void LogInfo(const csString &renderPass, const csString &msg);
  virtual void LogError(const csString &renderPass, const csString &msg);

};