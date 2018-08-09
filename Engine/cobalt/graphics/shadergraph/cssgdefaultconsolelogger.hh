#pragma once

#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>


class CSE_API csSGDefaultConsoleLogger : public iSGShaderGraphLogger
{
public:
  csSGDefaultConsoleLogger();
  virtual ~csSGDefaultConsoleLogger(); 

  virtual void LogSourceCode(const std::string &renderPass, const std::string &shaderType, const std::string &sourceCode);
  virtual void LogInfo(const std::string &renderPass, const std::string &msg);
  virtual void LogError(const std::string &renderPass, const std::string &msg);

};