
#include <valkyrie/graphics/shadergraph/vksgdefaultconsolelogger.hh>


vkSGDefaultConsoleLogger::vkSGDefaultConsoleLogger()
{

}

vkSGDefaultConsoleLogger::~vkSGDefaultConsoleLogger()
{

}

void vkSGDefaultConsoleLogger::LogSourceCode(const vkString &renderPass, const vkString &shaderType, const vkString &sourceCode)
{
  //printf("RenderPass: %s  ShaderType: %s\n%s\n", renderPass.c_str(), shaderType.c_str(), sourceCode.c_str());
}

void vkSGDefaultConsoleLogger::LogInfo(const vkString &renderPass, const vkString &msg)
{
  //printf("RenderPass: %s '%s'\n", renderPass.c_str(), msg.c_str());
}

void vkSGDefaultConsoleLogger::LogError(const vkString &renderPass, const vkString &msg)
{
  //fprintf(stderr, "RenderPass: %s\n%s\n", renderPass.c_str(), msg.c_str());
}
