

#include "codenodes.hh"


namespace cs::classgenerator
{

CodeNode::CodeNode()
{

}

void CodeNode::Add(CodeNode* node)
{
  if (node)
  {
    m_children.push_back(node);
  }
}

const std::vector<CodeNode*>& CodeNode::GetChildren() const
{
  return m_children;
}

void CodeNode::DebugNode(int ind)
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
  : CodeNode()
{

}

void BlockNode::Debug()
{
  printf("BLCK");
}

NamespaceNode::NamespaceNode()
  : CodeNode()
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
  : CodeNode()
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
  : CodeNode()
{

}
TokenNode::TokenNode(const Token& token)
  : CodeNode()
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