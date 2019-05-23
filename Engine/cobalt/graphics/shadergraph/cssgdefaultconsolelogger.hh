#pragma once

#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>

namespace cs
{

class CSE_API SGDefaultConsoleLogger : public cs::iSGShaderGraphLogger
{
public:
  SGDefaultConsoleLogger();
  virtual ~SGDefaultConsoleLogger();

  virtual void LogSourceCode(const std::string& renderPass, const std::string& shaderType, const std::string& sourceCode);
  virtual void LogInfo(const std::string& renderPass, const std::string& msg);
  virtual void LogError(const std::string& renderPass, const std::string& msg);

};

}
