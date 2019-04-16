

#include <ast.hh>


namespace cs::classgenerator
{

ASTNode::ASTNode(ASTNodeType type)
  : m_type(type)
  , m_parent(nullptr)
{

}

ASTNodeType ASTNode::GetType() const
{
  return m_type;
}

ASTNode* ASTNode::FindParentNode(ASTNodeType type)
{
  ASTNode* testNode = this;
  while (testNode)
  {
    if (testNode->GetType() == type)
    {
      return testNode;
    }
    testNode = testNode->GetParent();
  }
  return nullptr;
}

ASTNode* ASTNode::FindPrevSibling(ASTNodeType type)
{
  if (!m_parent)
  {
    return nullptr;
  }

  return nullptr;
}

ASTNode* ASTNode::FindNextSibling(ASTNodeType type)
{
  if (!m_parent)
  {
    return nullptr;
  }

  return nullptr;

}

void ASTNode::Add(ASTNode* node)
{
  if (node)
  {
    m_children.push_back(node);
    node->m_parent = this;
  }
}

ASTNode* ASTNode::GetParent()
{
  return m_parent;
}

const ASTNode* ASTNode::GetParent() const
{
  return m_parent;
}

const std::vector<ASTNode*>& ASTNode::GetChildren() const
{
  return m_children;
}

void ASTNode::DebugNode(int ind)
{
  for (int i = 0; i < ind; ++i)
  {
    printf("  ");
  }

  Debug();
  printf("\n");

  for (auto child : m_children)
  {
    child->DebugNode(ind + 1);
  }
}

BlockNode::BlockNode()
  : ASTNode(eANT_Block)
{

}

void BlockNode::Debug()
{
  printf("BLCK");
}

NamespaceNode::NamespaceNode()
  : ASTNode(eANT_Namespace)
{

}

void NamespaceNode::SetName(const std::string & name)
{
  m_name = name;
}

const std::string& NamespaceNode::GetName() const
{
  return m_name;
}

void NamespaceNode::SetAlias(const std::string & alias)
{
  m_alias = alias;
}

const std::string& NamespaceNode::GetAlias() const
{
  return m_alias;
}

void NamespaceNode::Debug()
{
  if (m_alias.empty())
  {
    printf("NS[%s]", m_name.c_str());
  }
  else
  {
    printf("NS[%s=%s]", m_alias.c_str(), m_name.c_str());
  }
}






ClassSuperDefinition::ClassSuperDefinition(bool csSuper, const std::string & name, const std::string & visibility, bool virtuality)
  : m_csSuper(csSuper)
  , m_name(name)
  , m_visibility(visibility)
  , m_virtual(virtuality)
{

}

bool ClassSuperDefinition::IsCSSuper() const
{
  return m_csSuper;
}

const std::string& ClassSuperDefinition::GetName() const
{
  return m_name;
}

const std::string& ClassSuperDefinition::GetVisibility() const
{
  return m_visibility;
}

bool ClassSuperDefinition::IsVirtual() const
{
  return m_virtual;
}

ClassNode::ClassNode()
  : ASTNode(eANT_Class)
{

}

void ClassNode::SetName(const std::string & name)
{
  m_name = name;
}

const std::string& ClassNode::GetName() const
{
  return m_name;
}

void ClassNode::AddSuper(const ClassSuperDefinition & super)
{
  m_supers.push_back(super);
}

const std::vector<ClassSuperDefinition>& ClassNode::GetSupers() const
{
  return m_supers;
}

void ClassNode::Debug()
{
  printf("CLS[%s", m_name.c_str());
  for (auto super : m_supers)
  {
    if (super.IsCSSuper())
    {
      printf(":CS_SUPER(%s%s%s)",
        super.IsVirtual() ? "virtual " : "",
        super.GetVisibility().empty() ? "" : (super.GetVisibility() + " ").c_str(),
        super.GetName().c_str());
    }
    else
    {

      printf(":%s%s%s",
        super.IsVirtual() ? "virtual " : "",
        super.GetVisibility().empty() ? "" : (super.GetVisibility() + " ").c_str(),
        super.GetName().c_str()
      );
    }
  }
  printf("]"); 
}


VisibilityNode::VisibilityNode(const std::string& visibility)
  : ASTNode(eANT_Visibility)
  , m_visibility(visibility)
{


}

const std::string& VisibilityNode::GetVisibility() const
{
  return m_visibility;
}

void VisibilityNode::Debug()
{
  printf("VSBLT[%s]", m_visibility.c_str());
}


Argument::Argument(const TypeDef& type, const std::string& name)
  : m_type(type)
  , m_name(name)
{

}

const TypeDef& Argument::GetType() const
{
  return m_type;
}

const std::string& Argument::GetName() const
{
  return m_name;
}

std::string Argument::GetText() const
{
  std::string text = m_type.GetText();
  if (!m_name.empty())
  {
    text += " " + m_name;
  }
  return text;
}

FunctionNode::FunctionNode()
  : ASTNode(eANT_Function)
  , m_name("")
  , m_virtual(false)
  , m_pureVirtual(false)
  , m_const (false)
{

}

void FunctionNode::SetName(const std::string& name)
{
  m_name = name;
}

const std::string& FunctionNode::GetName() const
{
  return m_name;
}

void FunctionNode::SetReturnValue(const TypeDef& returnValue)
{
  m_returnValue = returnValue;
}

const TypeDef& FunctionNode::GetReturnValue() const
{
  return m_returnValue;
}

void FunctionNode::SetVirtual(bool virtuality)
{
  m_virtual = virtuality;
}

bool FunctionNode::IsVirtual() const
{
  return m_virtual;
}

void FunctionNode::SetPureVirtual(bool pureVirtual)
{
  m_pureVirtual = pureVirtual;
}

bool FunctionNode::IsPureVirtual() const
{
  return m_pureVirtual;
}

void FunctionNode::SetConst(bool constness)
{
  m_const = constness;
}

bool FunctionNode::IsConst() const
{
  return m_const;
}

void FunctionNode::Add(const Argument& argument)
{
  m_arguments.push_back(argument);
}

void FunctionNode::Debug() 
{
  std::string arguments = "";
  for (size_t i = 0, in = m_arguments.size(); i < in; ++i)
  {
    arguments += m_arguments[i].GetText();
    if (i + 1 < m_arguments.size())
    {
      arguments += ", ";
    }
  }

  printf("FNCTN[%s%s%s%s(%s)]: %s",
    m_virtual ? "VIRTUAL " : "",
    m_name.c_str(),
    m_const ? " CONST" : "",
    m_pureVirtual ? " PURE " : "",
    arguments.c_str(),
    m_returnValue.GetText().c_str());


}


MemberNode::MemberNode()
  : ASTNode(eANT_Member)
{

}


void MemberNode::SetName(const std::string& name)
{
  m_name = name;
}

const std::string& MemberNode::GetName() const
{
  return m_name;
}

void MemberNode::SetType(const TypeDef& type)
{
  m_type = type;
}

const TypeDef& MemberNode::GetType() const
{
  return m_type;
}

void MemberNode::Debug()
{
  printf("MMBR [%s:%s]", 
    m_name.c_str(), m_type.GetText().c_str());
}



TypeDef::TypeDef()
  : m_const(false)
  , m_constPtr(false)
{

}

void TypeDef::Add(const Token& token)
{
  switch (token.GetType())
  {
  case eTT_Const:
    m_const = true;
    break;
  case eTT_Ampersand:
  case eTT_Asterisk:
  case eTT_DoubleAsterisk:
    m_constPtr = m_constPtr || m_const;
    m_const = false;
    m_mems.push_back(token);
    break;
  default:
    m_tokens.push_back(token);
  }
}

void TypeDef::Add(const TypeDef& subType)
{
  m_subTypes.push_back(subType);
}

void TypeDef::AddFront(const Token& token)
{
  switch (token.GetType())
  {
  case eTT_Const:
    m_const = true;
    break;
  case eTT_Ampersand:
  case eTT_Asterisk:
  case eTT_DoubleAsterisk:
    m_constPtr = m_constPtr || m_const;
    m_const = false;
    m_mems.insert(m_mems.begin(), token);
    break;
  default:
    m_tokens.insert(m_tokens.begin(), token);
  }
}

void TypeDef::AddFront(const TypeDef& subType)
{
  m_subTypes.insert(m_subTypes.begin(), subType);
}
void TypeDef::SetConst(bool constness)
{
  m_const = constness;
}

bool TypeDef::IsConst() const
{
  return m_const;
}

std::string TypeDef::GetText() const
{
  std::string text;
  if (m_const)
  {
    text += "const ";
  }
  for (auto tkn : m_tokens)
  {
    text += tkn.Get();
  }
  if (!m_subTypes.empty())
  {
    text += "<";
    for (size_t i = 0, in = m_subTypes.size(); i < in; ++i)
    {
      text += m_subTypes[i].GetText();
      if (i + 1 < in)
      {
        text += ", ";
      }
    }
    text += ">";
  }
  for (auto tkn : m_mems)
  {
    text += tkn.Get();
  }
  if (m_constPtr)
  {
    text += " const";
  }
  return text;
}




TokenNode::TokenNode()
  : ASTNode(eANT_Token)
{

}
TokenNode::TokenNode(const Token & token)
  : ASTNode(eANT_Token) 
  , m_token(token)
{

}

void TokenNode::SetToken(const Token & token)
{
  m_token = token;
}

const Token& TokenNode::GetToken() const
{
  return m_token;
}

void TokenNode::Debug()
{
  printf("TKN[%s]", m_token.Get().c_str());
}

}