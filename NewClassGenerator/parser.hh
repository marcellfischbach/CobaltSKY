

#pragma once


namespace cs::classgenerator
{


class NamespaceNode;
class BlockNode;
class CodeNode;
class Token;
class Tokenizer;
class Parser
{
public:
  Parser();

  CodeNode* Parse(Tokenizer& tokenizer);

private:
  BlockNode *ParseBlock(Tokenizer& tokenizer, size_t &idx);
  CodeNode *ParseNode(Tokenizer& tokenizer, Token &token, size_t& idx);
  NamespaceNode *ParseNamespace(Tokenizer& tokenizer, size_t& idx);

};


}