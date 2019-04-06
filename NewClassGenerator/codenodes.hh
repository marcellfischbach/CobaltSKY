

#pragma once

#include <vector>
#include <string>
#include "token.hh"

namespace cs::classgenerator
{


class CodeNode
{
public:
  CodeNode();

  void Add(CodeNode* codeNode);
  const std::vector<CodeNode*>& GetChildren() const;

  void DebugNode(int i);

protected:
  virtual void Debug() = 0;

private:

  std::vector<CodeNode*> m_children;
};


class BlockNode : public CodeNode
{
public:
  BlockNode();

protected:
  virtual void Debug();
};


class NamespaceNode : public CodeNode
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

class ClassNode : public CodeNode
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


class TokenNode : public CodeNode
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