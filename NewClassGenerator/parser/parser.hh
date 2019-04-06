

#pragma once


namespace cs::classgenerator
{


class ClassNode;
class NamespaceNode;
class BlockNode;
class ASTNode;
class Token;
class Tokenizer;
class Parser
{
public:
  Parser();

  ASTNode* Parse(Tokenizer& tokenizer);

private:
  BlockNode *ParseBlock(Tokenizer& tokenizer, size_t &idx);
  ASTNode *ParseNode(Tokenizer& tokenizer, Token &token, size_t& idx);
  NamespaceNode *ParseNamespace(Tokenizer& tokenizer, size_t& idx);
  ClassNode *ParseClass(Tokenizer& tokenizer, size_t& idx);

};


}