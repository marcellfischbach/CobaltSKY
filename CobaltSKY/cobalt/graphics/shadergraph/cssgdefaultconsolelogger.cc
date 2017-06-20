
#include <cobalt/graphics/shadergraph/cssgdefaultconsolelogger.hh>


csSGDefaultConsoleLogger::csSGDefaultConsoleLogger()
{

}

csSGDefaultConsoleLogger::~csSGDefaultConsoleLogger()
{

}

void csSGDefaultConsoleLogger::LogSourceCode(const csString &renderPass, const csString &shaderType, const csString &sourceCode)
{
  //printf("RenderPass: %s  ShaderType: %s\n%s\n", renderPass.c_str(), shaderType.c_str(), sourceCode.c_str());
}

void csSGDefaultConsoleLogger::LogInfo(const csString &renderPass, const csString &msg)
{
  //printf("RenderPass: %s '%s'\n", renderPass.c_str(), msg.c_str());
}

void csSGDefaultConsoleLogger::LogError(const csString &renderPass, const csString &msg)
{
  //fprintf(stderr, "RenderPass: %s\n%s\n", renderPass.c_str(), msg.c_str());
}
