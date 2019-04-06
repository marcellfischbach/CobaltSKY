

#include <parser/parser.hh>
#include <ast.hh>
#include <parser/tokenizer.hh>



namespace cs::classgenerator
{

Parser::Parser()
{


}


ASTNode* Parser::Parse(Tokenizer& tokenizer)
{
  size_t idx = 0;
  return ParseBlock(tokenizer, idx);
}

BlockNode *Parser::ParseBlock(Tokenizer& tokenizer, size_t &idx)
{
  BlockNode* blockNode = new BlockNode();
  auto tokens = tokenizer.GetTokens();
  for (size_t in = tokens.size(); idx < in;)
  {
    Token& token = tokens[idx];
    if (token.GetType() == eTT_CurlyBraceClose)
    {
      ++idx;
      break;
    }

    ASTNode* node = ParseNode(tokenizer, token, idx);
    if (node)
    {
      blockNode->Add(node);
    }
  }
  return blockNode;
}

ASTNode* Parser::ParseNode(Tokenizer& tokenizer, Token& token, size_t& idx)
{
  switch (token.GetType())
  {
  case eTT_CurlyBraceOpen:
    return ParseBlock(tokenizer, ++idx);
  case eTT_Namespace:
    return ParseNamespace(tokenizer, idx);
  case eTT_Class:
    return ParseClass(tokenizer, idx);

  default:
    ++idx;
    return new TokenNode(token);
  }
}

NamespaceNode* Parser::ParseNamespace(Tokenizer& tokenizer,  size_t& idx)
{
  NamespaceNode* namespaceNode = new NamespaceNode();
  std::string name = "";
  std::string alias = "";
  auto tokens = tokenizer.GetTokens();
  BlockNode* blockNode;
  bool namespaceDone = false;
  for (size_t i = idx + 1, in = tokens.size(); i < in && !namespaceDone; ++i)
  {
    Token& token = tokens[i];
    switch (token.GetType())
    {
    case eTT_CurlyBraceOpen:
      blockNode = ParseBlock(tokenizer, ++i);
      if (blockNode)
      {
        namespaceNode->Add(blockNode);
      }
      namespaceDone = true;
      // when read block returns the index (i) already points to the next token
      idx = i;
      break;
    case eTT_SemiColon:
      // when ending with a semicolon in index must be pointing to the next token (i+1)
      namespaceDone = true;
      idx = i + 1;
      break;
    case eTT_Equal:
      alias = name;
      name = "";
      break;
    case eTT_Identifier:
    case eTT_DoubleColon:
      name += token.Get();
      break;
    }

  }
  namespaceNode->SetName(name);
  namespaceNode->SetAlias(alias);

  return namespaceNode;
}

ClassNode* Parser::ParseClass(Tokenizer& tokenizer, size_t& idx)
{
  ClassNode* classNode = new ClassNode();

  return classNode;
}


}