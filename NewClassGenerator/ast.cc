

#include <ast.hh>


namespace cs::classgenerator
{

ASTNode::ASTNode(ASTNodeType type)
  : m_type(type)
{

}

ASTNodeType ASTNode::GetType() const
{
  return m_type;
}

void ASTNode::Add(ASTNode* node)
{
  if (node)
  {
    m_children.push_back(node);
  }
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

void NamespaceNode::SetName(const std::string& name)
{
  m_name = name;
}

const std::string& NamespaceNode::GetName() const
{
  return m_name;
}

void NamespaceNode::SetAlias(const std::string& alias)
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



ClassNode::ClassNode()
  : ASTNode(eANT_Class)
{

}

void ClassNode::SetName(const std::string& name)
{
  m_name = name;
}

const std::string& ClassNode::GetName() const
{
  return m_name;
}

void ClassNode::Debug()
{
  printf("CLS[%s]", m_name.c_str());
}



TokenNode::TokenNode()
  : ASTNode(eANT_Token)
{

}
TokenNode::TokenNode(const Token& token)
  : ASTNode(eANT_Token)
  , m_token(token)
{

}

void TokenNode::SetToken(const Token& token)
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