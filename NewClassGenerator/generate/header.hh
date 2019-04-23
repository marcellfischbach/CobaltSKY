
#pragma once

#include <generate/generator.hh>

namespace cs::classgenerator
{

class ClassNode;
class HeaderGenerator : public Generator
{
public:
  HeaderGenerator();

  void Output(const std::string& fileName, const std::string& exp);

private:
  std::string OutputClass(ClassNode* classNode, const std::string & exp);
};
}