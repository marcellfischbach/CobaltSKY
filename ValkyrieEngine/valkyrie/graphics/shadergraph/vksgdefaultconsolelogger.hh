#pragma once

#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>


class VKE_API vkSGDefaultConsoleLogger : public iSGShaderGraphLogger
{
public:
  vkSGDefaultConsoleLogger();
  virtual ~vkSGDefaultConsoleLogger(); 

  virtual void LogSourceCode(const vkString &renderPass, const vkString &shaderType, const vkString &sourceCode);
  virtual void LogInfo(const vkString &renderPass, const vkString &msg);
  virtual void LogError(const vkString &renderPass, const vkString &msg);

};