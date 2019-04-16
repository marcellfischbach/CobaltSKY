

#pragma once

#include <list>
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
  eANT_Visibility,
  eANT_Function,
  eANT_Member,
  eANT_Token
};

class ASTNode
{
public:

  void Add(ASTNode* codeNode);
  const std::vector<ASTNode*>& GetChildren() const;

  ASTNode* GetParent();
  const ASTNode* GetParent() const;

  ASTNodeType GetType() const;

  ASTNode* FindParentNode(ASTNodeType type);
  ASTNode* FindPrevSibling(ASTNodeType type);
  ASTNode* FindNextSibling(ASTNodeType type);

  void DebugNode(int i);

  
protected:
  ASTNode(ASTNodeType type);
  virtual void Debug() = 0;

private:

  ASTNodeType m_type;

  ASTNode* m_parent;
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

class ClassSuperDefinition
{
public:
  ClassSuperDefinition(bool csSuper, const std::string& name, const std::string& visibility, bool virtuality);

  bool IsCSSuper() const;
  const std::string& GetName() const;
  const std::string& GetVisibility() const;
  bool IsVirtual() const;

private:
  bool m_csSuper;
  std::string m_name;
  std::string m_visibility;
  bool m_virtual;
};

class ClassNode : public ASTNode
{
public:
  ClassNode();

  void SetName(const std::string& name);
  const std::string& GetName() const;

  void AddSuper(const ClassSuperDefinition& super);
  const std::vector<ClassSuperDefinition>& GetSupers() const;

protected:
  virtual void Debug();

private:
  std::string m_name;

  std::vector<ClassSuperDefinition> m_supers;
};

class VisibilityNode : public ASTNode
{
public:
  VisibilityNode(const std::string& visibility);

  const std::string& GetVisibility() const;
protected:
  virtual void Debug();

private:
  std::string m_visibility;
};

class TypeDef
{
public:
  TypeDef();

  void Add(const Token& token);
  void Add(const TypeDef& subType);

  void AddFront(const Token& token);
  void AddFront(const TypeDef& subType);
  void SetConst(bool constness);
  bool IsConst() const;

  std::string GetText() const;

private:
  std::vector<Token> m_tokens;
  std::vector<Token> m_mems;
  std::vector<TypeDef> m_subTypes;

  bool m_const;
  bool m_constPtr;

};

class Argument
{
public:
  Argument(const TypeDef& typeDef, const std::string& name = "");
  const TypeDef& GetType() const;
  const std::string& GetName() const;

  std::string GetText() const;

private:
  TypeDef m_type;
  std::string m_name;

};


class FunctionNode : public ASTNode
{
public:
  FunctionNode();

  void SetName(const std::string& name);
  const std::string& GetName() const;

  void SetReturnValue(const TypeDef& returnValue);
  const TypeDef &  GetReturnValue() const;

  void SetPureVirtual(bool pureVirtual);
  bool IsPureVirtual() const;

  void SetVirtual(bool virtuality);
  bool IsVirtual() const;

  void SetConst(bool constness);
  bool IsConst() const;

  void Add(const Argument& argument);
protected:
  virtual void Debug();


private:
  TypeDef m_returnValue;
  bool m_virtual;
  bool m_pureVirtual;
  bool m_const;


  std::string m_name;

  std::vector<Argument> m_arguments;
};


class MemberNode : public ASTNode
{
public:
  MemberNode();

  void SetName(const std::string& name);
  const std::string& GetName() const;

  void SetType(const TypeDef& typeDef);
  const TypeDef& GetType() const;

protected:
  virtual void Debug();


private:
  TypeDef m_type;

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