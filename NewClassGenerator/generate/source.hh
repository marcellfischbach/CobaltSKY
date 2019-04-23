
#pragma once


#include <generate/generator.hh>

namespace cs::classgenerator
{

class ClassNode;
class FunctionNode;
class MemberNode;

class SourceGenerator : public Generator
{
public:
  SourceGenerator();

  void Output(const std::string& fileName, const std::string& exp);

private:
  std::string OutputClass(ClassNode* classNode);
  std::string GenerateAttribute(ClassNode* classNode, MemberNode* memberNode);
  std::string GenerateFunction(ClassNode *classNode, FunctionNode* functionNode);
};


}
