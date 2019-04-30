
#include <generate/header.hh>
#include <generate/ioutput.hh>
#include <ast.hh>

namespace cs::classgenerator
{


HeaderGenerator::HeaderGenerator()
{

}

void HeaderGenerator::Output(const std::string& filename, const std::string& exp, iOutput* output)
{
  std::vector<ClassNode*> classes = FindAllMajorClasses();


  for (auto cls : classes)
  {
    ASTNode* prev = cls->FindPrevSibling();
    if (prev == nullptr || !prev->GetType() == eANT_CSMeta)
    {
      continue;
    }

    CSMetaNode* meta = static_cast<CSMetaNode*>(prev);
    if (meta->GetMetaType() != CSMetaNode::eMT_Class)
    {
      continue;
    }

    std::string clsSource = OutputClass(cls, exp);
    if (output)
    {
      output->output(clsSource);
    }
  }
}

std::string HeaderGenerator::OutputClass(ClassNode* clsNode, const std::string& exp)
{
  std::string source;
  ASTNode* parent = clsNode;
  std::list<NamespaceNode*> nss = GetAllNamespaces(clsNode);

  std::string clsName = clsNode->GetName() + "Class";

  source += "#pragma once\n";
  source += "\n";
  for (auto ns : nss)
  {
    source += "namespace " + ns->GetName() + "\n";
    source += "{\n";
  }

  source += "\nclass " + exp + " " + clsName + " : public csClass\n";
  source += "{\n";
  source += "public:\n";
  source += " " + clsName + "();\n";
  source += " static " + clsName  + "() *Get();\n";
  source += " virtual iObject *CreateInstance() const;\n";
  source += "};\n";


  for (auto ns : nss)
  {
    source += "}\n";
  }
  source += "\n";
  return source;
}
}