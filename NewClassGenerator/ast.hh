

#pragma once

#include <vector>
#include <string>
#include <parser/token.hh>

namespace cs::classgenerator
{

enum ASTNodeType
{
  eANT_Block,
  eANT_Class,
  eANT_Namespace,
  eANT_Token
};

class ASTNode
{
public:

  void Add(ASTNode* codeNode);
  const std::vector<ASTNode*>& GetChildren() const;

  ASTNodeType GetType() const;

  void DebugNode(int i);

protected:
  ASTNode(ASTNodeType type);
  virtual void Debug() = 0;

private:

  ASTNodeType m_type;

  std::vector<ASTNode*> m_children;
};


class BlockNode : public ASTNode
{
public:
  BlockNode();

protected:
  virtual void Debug();
};


class NamespaceNode : public ASTNode
{
public:
  NamespaceNode();

  void SetName(const std::string& name);
  const std::string& GetName() const;

  void SetAlias(const std::string& alias);
  const std::string& GetAlias() const;
protected:
  virtual void Debug();


private:
  std::string m_name;
  std::string m_alias;
};

class ClassNode : public ASTNode
{
public:
  ClassNode();

  void SetName(const std::string& name);
  const std::string& GetName() const;

protected:
  virtual void Debug();

private:
  std::string m_name;

};


class TokenNode : public ASTNode
{
public:
  TokenNode();
  TokenNode(const Token& token);

  void SetToken(const Token& token);
  const Token &GetToken() const;

protected:
  virtual void Debug();

private:
  Token m_token;
};


}