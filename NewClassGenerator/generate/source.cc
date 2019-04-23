
#include <generate/source.hh>
#include <ast.hh>

namespace cs::classgenerator
{

SourceGenerator::SourceGenerator()
  : Generator()
{

}

void SourceGenerator::Output(const std::string& fileName, const std::string& exp)
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

    std::string clsSource = OutputClass(cls);
    printf("Source:\n%s\n", clsSource.c_str());
  }

}


std::string SourceGenerator::OutputClass(ClassNode* classNode)
{

  std::string source;
  std::list<NamespaceNode*> nss = GetAllNamespaces(classNode);

  for (auto ns : nss)
  {
    source += "namespace " + ns->GetName() + "\n";
    source += "{\n";
  }

  ASTNode* childBlock = classNode->GetChildren().size() >= 1 ? classNode->GetChildren()[0] : nullptr;
  if (!childBlock || childBlock->GetType() != eANT_Block)
  {
    return source;
  }


  std::string visibility = "private";
  CSMetaNode* lastMeta = nullptr;
  for (auto child : childBlock->GetChildren())
  {
    if (child->GetType() == eANT_Visibility)
    {
      visibility = static_cast<VisibilityNode*>(child)->GetVisibility();
    }
    else if (child->GetType() == eANT_CSMeta)
    {
      lastMeta = static_cast<CSMetaNode*>(child);
    }
    else if (child->GetType() == eANT_Member)
    {
      if (lastMeta && lastMeta->GetMetaType() == CSMetaNode::eMT_Property)
      {
        source += GenerateAttribute(classNode, static_cast<MemberNode*>(child));
      }
    }
    else if (child->GetType() == eANT_Function)
    {
      if (lastMeta && lastMeta->GetMetaType() == CSMetaNode::eMT_Function)
      {
        source += GenerateFunction(classNode, static_cast<FunctionNode*>(child));
      }
    }


    if (child->GetType() != eANT_CSMeta)
    {
      lastMeta = nullptr;
    }
  }


  for (auto ns : nss)
  {
    source += "}\n";
  }

  return source;
}

std::string GetValueDeclarationMem(TypeDef& typeDef)
{
  if (typeDef.IsReference())
  {
    return "eVMM_Reference";
  }
  else if (typeDef.IsPointer())
  {
    return "eVMM_Pointer";
  }
  else if (typeDef.IsPointerToPointer())
  {
    return "eVMM_PointerToPointer";
  }
  return "eVMM_Value";
}

std::string SourceGenerator::GenerateFunction(ClassNode *classNode, FunctionNode* function)
{

  std::string fnctClassName = classNode->GetName() + "Class_" + function->GetName();
  if (function->IsConst())
  {
    fnctClassName += "_Const";
  }

  std::string source = "";
  source +=  "class " + fnctClassName + " : public csFunction\n";
  source += "{\n";
  source += "public:\n";

  // create the constructor
  source += "  " + fnctClassName + "()\n";
  source += "  : csFunction (" + 
    (function->IsVirtual() ? "eV_Virtual, " : "eVT_NonVirtual, ") + 
    "csValueDeclaration (" + 


  source += "";
  source += "};\n";
  return source;
}

std::string SourceGenerator::GenerateAttribute(ClassNode* classNode, MemberNode* member)
{
  return "";
}

}