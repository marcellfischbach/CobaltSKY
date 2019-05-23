
#include <cobalt/graphics/shadergraph/cssgdefaultconsolelogger.hh>


cs::SGDefaultConsoleLogger::SGDefaultConsoleLogger()
{

}

cs::SGDefaultConsoleLogger::~SGDefaultConsoleLogger()
{

}

void cs::SGDefaultConsoleLogger::LogSourceCode(const std::string &renderPass, const std::string &shaderType, const std::string &sourceCode)
{
  //printf("RenderPass: %s  ShaderType: %s\n%s\n", renderPass.c_str(), shaderType.c_str(), sourceCode.c_str());
}

void cs::SGDefaultConsoleLogger::LogInfo(const std::string &renderPass, const std::string &msg)
{
  //printf("RenderPass: %s '%s'\n", renderPass.c_str(), msg.c_str());
}

void cs::SGDefaultConsoleLogger::LogError(const std::string &renderPass, const std::string &msg)
{
  //fprintf(stderr, "RenderPass: %s\n%s\n", renderPass.c_str(), msg.c_str());
}
