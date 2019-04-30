
#include <generate/source.hh>
#include <generate/ioutput.hh>
#include <ast.hh>

namespace cs::classgenerator
{

SourceGenerator::SourceGenerator()
  : Generator()
{

}

void SourceGenerator::Output(const std::string& fileName, const std::string& exp, iOutput *output)
{

  std::vector<ClassNode*> classes = FindAllMajorClasses();

  std::string source;


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

    std::list<NamespaceNode*> nss = GetAllNamespaces(cls);

    for (auto ns : nss)
    {
      source += "namespace " + ns->GetName() + "\n";
      source += "{\n";
    }


    ClassGenerator classGenerator;
    source = classGenerator.OutputClass(cls, meta);


    for (auto ns : nss)
    {
      source += "}\n";
    }

  }

  if (output)
  {
    output->output(source);
  }
}


std::string ClassGenerator::OutputClass(ClassNode* classNode, CSMetaNode *meta)
{

  std::string source;
  
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
        source += GenerateAttribute(classNode, static_cast<MemberNode*>(child), lastMeta);
      }
    }
    else if (child->GetType() == eANT_Function)
    {
      if (lastMeta && lastMeta->GetMetaType() == CSMetaNode::eMT_Function)
      {
        source += GenerateFunctionClass(classNode, static_cast<FunctionNode*>(child), lastMeta);
      }
    }


    if (child->GetType() != eANT_CSMeta)
    {
      lastMeta = nullptr;
    }
  }

  source += GenerateClass(classNode, meta);


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

std::string ClassGenerator::GenerateFunctionClass(ClassNode* classNode, FunctionNode* function, CSMetaNode *meta)
{

  std::string fnctClassName = classNode->GetName() + "Class_" + function->GetName();
  if (function->IsConst())
  {
    fnctClassName += "_Const";
  }

  m_functionClasses.push_back(fnctClassName);

  std::string source = "";
  source += "class " + fnctClassName + " : public csFunction\n";
  source += "{\n";
  source += "public:\n";

  // create the constructor
  source += "  " + fnctClassName + "()\n";
  source += std::string("    : csFunction (") +
    (function->IsVirtual() ? "eFV_Virtual, " : "eFV_NonVirtual, ") +
    GenerateCSValueDeclaration(function->GetReturnValue()) + ", " +
    "\"" + function->GetName() + "\", " +
    (function->IsConst() ? "eC_Const" : "eC_NonConst") +
    ")\n";
  source += "  {\n";

  for (auto argument : function->GetArguments())
  {
    source += "    " + GenerateAddAttribute(argument) + ";\n";
  }

  source += "  }\n";


  source += GenerateFunctionInvokeMethod(classNode, function, false);
  source += GenerateFunctionInvokeMethod(classNode, function, true);





  source += "";
  source += "};\n\n\n";
  return source;
}

std::string ClassGenerator::GenerateFunctionInvokeMethod(ClassNode* classNode, FunctionNode* function, bool _const)
{
  std::string fkt;
  std::string sConst = _const ? "const " : "";

  fkt += "\n";
  fkt += "  virtual void Invoke (" + sConst + "iObject* obj, ...) const\n";
  fkt += "  {\n";
  if (!_const || function->IsConst())
  {
    fkt += "    " + sConst + classNode->GetName() + " *d = csQueryClass<" + sConst + classNode->GetName() + ">(obj);\n";
    fkt += "    if (!d)\n";
    fkt += "    {\n";
    fkt += "      return;\n";
    fkt += "    }\n\n";
    fkt += "    va_list lst;\n";
    fkt += "    va_start(lst, obj);\n";
    const std::vector<Argument>& arguments = function->GetArguments();
    for (auto argument : arguments)
    {
      std::string type = GenerateTypeForMethodInvokation(argument.GetType());
      fkt += "    " + type + argument.GetName() + " = va_arg(lst, " + type + ");\n";

    }
    bool isNotVoid = !function->GetReturnValue().IsVoid();

    if (isNotVoid)
    {
      std::string type = GenerateTypeForMethodReturnInvokation(function->GetReturnValue());
      fkt += "    " + type +"____ptr____result____ = va_arg(lst, " + type + ");\n";
    }
    fkt += "    va_end(lst);\n";
    fkt += "    ";
    if (isNotVoid)
    {
      fkt += function->GetReturnValue().GetText() + " ____value____result___ = ";
    }
    fkt += "d->" + function->GetName() + "(";

    for (size_t i=0, in = arguments.size(); i<in; ++i)
    {
      const Argument &argument = arguments[i];
      fkt += "*" + argument.GetName();
      if (i + 1 < in)
      {
        fkt += ", ";
      }
    }
    fkt += ");\n";

    fkt += "    if (____ptr____result____)\n";
    fkt += "    {\n";
    fkt += "      *____ptr____result____ = &____value____result___;\n";
    fkt += "    }\n";
  }

  fkt += "  }\n";
  return fkt;
}

std::string ClassGenerator::GenerateTypeForMethodInvokation(const TypeDef& typeDef)
{

  std::string type;
  if (typeDef.IsConst())
  {
    type = "const ";
  }
  type += typeDef.GetTypeName() + " ";
  if (typeDef.IsValue() || typeDef.IsReference()) {
    type += "*";
  }
  else if (typeDef.IsPointer())
  {
    type += "**";
  }
  else if (typeDef.IsPointerToPointer())
  {
    type += "***";
  }

  return type;
}

std::string ClassGenerator::GenerateTypeForMethodReturnInvokation(const TypeDef& typeDef)
{

  std::string type;
  if (typeDef.IsConst())
  {
    type = "const ";
  }
  type += typeDef.GetTypeName() + " ";
   if (typeDef.IsValue()) {
    type += "*";
  }
  else if (typeDef.IsReference() ||typeDef.IsPointer())
  {
    type += "**";
  }
  else if (typeDef.IsPointerToPointer())
  {
    type += "***";
  }
  return type;
}


std::string ClassGenerator::GenerateAttribute(ClassNode* classNode, MemberNode* member, CSMetaNode *meta)
{
  std::string propName = member->GetName();
  if (propName._Starts_with("m_"))
  {
    propName = propName.substr(2);
  }
  else if (propName._Starts_with("_"))
  {
    propName = propName.substr(1);
  }
  if (propName[0] >= 'a' && propName[0] <= 'z')
  {
    propName[0] += 'A' - 'a';
  }
  std::string propClassName = classNode->GetName() + "Class_" + propName;



  TypeDef type = member->GetType();
  TypeDef containerType;
  if (!type.GetSubTypes().empty())
  {
    containerType = type;
    if (type.GetSubTypes().size() > 1)
    {
      return "";
    }
    type = type.GetSubTypes()[0];
  }

  m_propertyClasses.push_back(propClassName);

  std::string prop;
  prop += "class " + propClassName + " : public csProperty\n";
  prop += "{\n";
  prop += "public:\n";
  prop += "  " + propClassName + "()\n";
  prop += "    : csProperty(" + GenerateCSValueDeclaration(containerType, false) + ", \"" + propName + "\", " + GenerateCSValueDeclaration(type, true) + ")\n";
  prop += "  {\n";
  prop += "  }\n\n";

  prop += "  virtual void SetValue (iObject *object, void *data) const\n";
  prop += "  {\n";
  prop += "  }\n\n";

  prop += "  virtual void* GetValue (const iObject *object) const\n";
  prop += "  {\n";
  prop += "    return nullptr;\n";
  prop += "  }\n\n";

  prop += "};\n\n";

  return prop;
}


std::string ClassGenerator::GenerateCSValueDeclaration(const TypeDef& typeDef, bool withSubTypes)
{
  std::string evmm = "eVMM_Value";
  if (typeDef.IsReference())
  {
    evmm = "eVMM_Reference";
  }
  else if (typeDef.IsPointer())
  {
    evmm = "eVMM_Pointer";
  }
  else if (typeDef.IsPointerToPointer())
  {
    evmm = "eVMM_PointerToPointer";
  }


  return std::string("csValueDeclaration(") +
    (typeDef.IsConst() ? "eC_Const" : "eC_NonConst") + ", " +
    "\"" + typeDef.GetTypeName(withSubTypes) + "\", " +
    evmm + ")";
}

std::string ClassGenerator::GenerateAddAttribute(const Argument& argument)
{
  return std::string("AddAttribute(csFunctionAttribute (") +
    GenerateCSValueDeclaration(argument.GetType()) + ", " +
    "\"" + argument.GetName() + "\"))";

}


std::string ClassGenerator::GenerateClass(ClassNode* classNode, CSMetaNode *meta)
{
  std::string cls;

  std::string className = classNode->GetName();
  std::string classClassName = classNode->GetName() + "Class";

  //
  // Singleton getter
  cls += classClassName + " *" + classClassName + "::Get()\n";
  cls += "{\n";
  cls += "  static " + classClassName + " static_class;\n";
  cls += "  return &static_class;\n";
  cls += "};\n\n";

  // 
  // Class constructor
  cls += classClassName + "::" + classClassName + "()\n";
  cls += "  : csClass(\"" + className + "\")\n";
  cls += "{\n";
  for (auto super : classNode->GetSupers())
  {
    if (super.IsCSSuper())
    {
      cls += "  AddSuperClass(" + super.GetType().GetTypeName() + "Class::Get());\n";
    }
  }

  for (auto propertyName : m_propertyClasses)
  {
    cls += "  AddProperty(new " + propertyName + "());\n";
  }

  for (auto functionName : m_functionClasses)
  {
    cls += "  AddFunction(new " + functionName + "());\n";
  }
  
  cls += "}\n\n";

  cls += "iObject *" + classClassName + "::CreateInstance() const\n";
  cls += "{\n";
  if (!classNode->HasPureVirtualMethod() && classNode->HasPublicDefaultConstructor() && !meta->Has("NoInstance"))
  {
    cls += "  static_cast<iObject*>(new " + className + "());\n";
  }
  cls += "}\n\n";


  cls += GenerateQueryClass(classNode, meta, false);
  cls += GenerateQueryClass(classNode, meta, true);


  return cls;

}


std::string ClassGenerator::GenerateQueryClass(ClassNode* classNode, CSMetaNode* meta, bool _const)
{
  std::string query = "";
  std::string sConst = _const ? "const " : "";
  std::string className = classNode->GetName();
  std::string classClassName = classNode->GetName() + "Class";

  query += sConst + "void *" + className + "::QueryClass(const csClass* clazz) " + sConst + "\n";
  query += "{\n";
  query += "  if (clazz == " + classClassName + "::Get())\n";
  query += "  {\n";
  query += "    return static_cast<" + sConst + className + "*>(this);\n";
  query += "  }\n";
  query += "  " + sConst + "void *super = nullptr;\n";

  for (auto super : classNode->GetSupers())
  {
    if (!super.IsCSSuper())
    {
      continue;
    }

    query += "  super = " + super.GetType().GetTypeName() + "::QueryClass(clazz);\n";
    query += "  if (super)\n";
    query += "  {\n";
    query += "    return super;\n";
    query += "  }\n";

  }
  
  query += "  return nullptr;\n";
  query += "}\n\n";
  return query;
}

}