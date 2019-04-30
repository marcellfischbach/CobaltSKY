
#pragma once


#include <generate/generator.hh>
#include <vector>

namespace cs::classgenerator
{

struct iOutput;
class CSMetaNode;
class Argument;
class ClassNode;
class FunctionNode;
class MemberNode;
class TypeDef;

class SourceGenerator : public Generator
{
public:
  SourceGenerator();

  void Output(const std::string& fileName, const std::string& exp, iOutput *output);

private:
};

class ClassGenerator
{
public:
  std::string OutputClass(ClassNode* classNode, CSMetaNode* metaNode);

private:
  std::string GenerateAttribute(ClassNode* classNode, MemberNode* memberNode, CSMetaNode *metaNode);


  std::string GenerateFunctionClass(ClassNode *classNode, FunctionNode* functionNode, CSMetaNode *meta);
  std::string GenerateFunctionInvokeMethod(ClassNode* classNode, FunctionNode* functionNode, bool _const);
  std::string GenerateTypeForMethodInvokation(const TypeDef& typeDef);
  std::string GenerateTypeForMethodReturnInvokation(const TypeDef& typeDef);


  std::string GenerateCSValueDeclaration(const TypeDef& typeDef, bool withSubTypes = true);
  std::string GenerateAddAttribute(const Argument& argument);

  std::string GenerateClass(ClassNode* classNode, CSMetaNode *meta);
  std::string GenerateQueryClass(ClassNode* classNode, CSMetaNode* meta, bool _const);

private:
  std::vector<std::string> m_propertyClasses;
  std::vector<std::string> m_functionClasses;
};


}
