

#include <parser/parseexception.hh>
#include <parser/parser.hh>
#include <ast.hh>
#include <parser/tokenizer.hh>
#include <deque>
#include <list>


namespace cs::classgenerator
{

Parser::Parser()
{


}


ASTNode* Parser::Parse(Tokenizer& tokenizer)
{
  size_t idx = 0;
  return ParseBlock(tokenizer, idx, nullptr);
}

BlockNode* Parser::ParseBlock(Tokenizer& tokenizer, size_t& idx, ASTNode* parent)
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

    ASTNode* node = ParseNode(tokenizer, token, idx, blockNode);
    if (node)
    {
      blockNode->Add(node);
    }
  }
  return blockNode;
}

ASTNode* Parser::ParseNode(Tokenizer& tokenizer, Token& token, size_t& idx, ASTNode* parent)
{
  switch (token.GetType())
  {
  case eTT_CurlyBraceOpen:
    return ParseBlock(tokenizer, ++idx, parent);
  case eTT_Namespace:
    return ParseNamespace(tokenizer, idx, parent);
  case eTT_Class:
    return ParseClass(tokenizer, idx, parent);

  case eTT_Public:
  case eTT_Private:
  case eTT_Protected:
    return ParseVisibility(tokenizer, idx, parent);

  default:
    idx++;
    return  nullptr;

  }
}

NamespaceNode* Parser::ParseNamespace(Tokenizer& tokenizer, size_t& idx, ASTNode* parent)
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
      blockNode = ParseBlock(tokenizer, ++i, namespaceNode);
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

ClassNode* Parser::ParseClass(Tokenizer& tokenizer, size_t& idx, ASTNode* parent)
{
  ClassNode* classNode = new ClassNode();

  std::string name = "";
  std::string alias = "";
  auto tokens = tokenizer.GetTokens();
  BlockNode* blockNode;

  enum ClassParseState
  {
    eCPS_Name,
    eCPS_Super,
  };


  ClassParseState state = eCPS_Name;

  for (size_t i = idx + 1, in = tokens.size(); i < in; ++i)
  {
    Token& token = tokens[i];
    switch (state)
    {

    case eCPS_Name:
      switch (token.GetType())
      {
      case eTT_SemiColon:
        classNode->SetName(tokens[i - 1].Get());
        idx = i + 1;
        return classNode;


      case eTT_Colon:
        classNode->SetName(tokens[i - 1].Get());
        state = eCPS_Super;
        break;

      case eTT_CurlyBraceOpen:
        classNode->SetName(tokens[i - 1].Get());

        blockNode = ParseClassBlock(tokenizer, ++i, classNode);
        if (blockNode)
        {
          classNode->Add(blockNode);
        }
        // when read block returns the index (i) already points to the next token
        idx = i;
        return classNode;
      }
      break;




    case eCPS_Super:
      switch (token.GetType())
      {

      case eTT_CurlyBraceOpen:
      {
        ClassSuperDefinition superDefinition = GetSuperDefinition(tokenizer, i - 1, classNode);
        classNode->AddSuper(superDefinition);
        blockNode = ParseClassBlock(tokenizer, ++i, classNode);
        if (blockNode)
        {
          classNode->Add(blockNode);
        }
        // when read block returns the index (i) already points to the next token
        idx = i;
        return classNode;

      }
      case eTT_Comma:
        ClassSuperDefinition superDefinition = GetSuperDefinition(tokenizer, i - 1, classNode);
        classNode->AddSuper(superDefinition);
        break;
      }
    }
  };
  return classNode;
}


BlockNode* Parser::ParseClassBlock(Tokenizer& tokenizer, size_t& idx, ASTNode* parent)
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

    size_t IDX = idx;
    ASTNode* node = ParseClassNode(tokenizer, token, idx, blockNode);
    if (node)
    {
      blockNode->Add(node);
    }
    if (IDX == idx)
    {
      break;
    }
  }
  return blockNode;
}

ASTNode* Parser::ParseClassNode(Tokenizer& tokenizer, Token& token, size_t& idx, ASTNode* parent)
{
  switch (token.GetType())
  {
  case eTT_CurlyBraceOpen:
    return ParseBlock(tokenizer, ++idx, parent);
  case eTT_Namespace:
    return ParseNamespace(tokenizer, idx, parent);
  case eTT_Class:
    return ParseClass(tokenizer, idx, parent);

  case eTT_Public:
  case eTT_Private:
  case eTT_Protected:
    return ParseVisibility(tokenizer, idx, parent);

  default:
    return ParseFunctionOrMember(tokenizer, idx, parent);

  }
}


ClassSuperDefinition Parser::GetSuperDefinition(Tokenizer& tokenizer, size_t idx, ASTNode* parent)
{
  auto tokens = tokenizer.GetTokens();

  std::string name;
  std::string visibility = "";
  bool virtuality = false;
  bool csSuper = false;

  size_t i = idx;
  for (; i > 0; --i)
  {
    Token& token = tokens[i];
    switch (token.GetType())
    {
    case eTT_Colon:
    case eTT_Comma:
      return ClassSuperDefinition(csSuper, name, visibility, virtuality);

    case eTT_Public:
      visibility = "public";
      break;
    case eTT_Protected:
      visibility = "protected";
      break;
    case eTT_Private:
      visibility = "private";
      break;
    case eTT_Virtual:
      virtuality = true;
      break;
    case eTT_Identifier:
      if (token.Get() == "CS_SUPER" && tokens.size() > i + 2)
      {
        if (tokens[i + 1].GetType() == eTT_ParenOpen)
        {
          csSuper = true;
          name = tokens[i + 2].Get();
        }
      }
      else
      {
        name = token.Get();
      }
      break;
    case eTT_Class:
      throw ParseException();
    }
  }

  throw ParseException();

}



VisibilityNode* Parser::ParseVisibility(Tokenizer& tokenizer, size_t& idx, ASTNode* parent)
{
  if (tokenizer.GetTokens()[idx + 1].GetType() != eTT_Colon)
  {
    throw ParseException();
  }
  idx += 2;
  return new VisibilityNode(tokenizer.GetTokens()[idx - 2].Get());
}


ASTNode* Parser::ParseFunctionOrMember(Tokenizer & tokenizer, size_t & idx, ASTNode * parent)
{

  auto tokens = tokenizer.GetTokens();
  for (size_t i = idx, in = tokens.size(); i < in; ++i)
  {
    Token& token = tokens[i];
    if (token.GetType() == eTT_ParenOpen)
    {
      return ParseFunction(tokenizer, idx, parent);
    }
    else if (token.GetType() == eTT_SemiColon)
    {
      return ParseMember(tokenizer, idx, parent);
    }
    else if (token.GetType() == eTT_Enum)
    {
      size_t idxCB = tokenizer.Find(eTT_CurlyBraceOpen, i);
      SkipBlock(tokenizer, idxCB);
      idx = idxCB;
      return nullptr;
    }
    else if (token.GetType() == eTT_CurlyBraceOpen)
    {
      size_t idxCB = tokenizer.Find(eTT_CurlyBraceClose, i);
      idx = idxCB + 1;
      return nullptr;

    }
  }


  idx++;
  return new TokenNode(tokens[idx - 1]);
}

FunctionNode* Parser::ParseFunction(Tokenizer & tokenizer, size_t & idx, ASTNode * parent)
{
  auto tokens = tokenizer.GetTokens();

  size_t idxParen = tokenizer.Find(eTT_ParenOpen, idx);
  size_t lIDX = idxParen;
  std::string name = ReverseName(tokenizer, lIDX);
  TypeDef def;
  if (lIDX >= idx)
  {
    size_t defIdx = idx;
    def = GetType(tokenizer, defIdx);
  }

  FunctionNode* func = new FunctionNode();
  func->SetName(name);
  func->SetReturnValue(def);

  try
  {
    size_t virtualIdx = tokenizer.Find(eTT_Virtual, idx);
    if (virtualIdx < idxParen)
    {
      func->SetVirtual(true);
    }
  }
  catch (std::exception & e)
  {
    //
  }

  idx = idxParen + 1;
  for (size_t in = tokens.size(); idx < in;)
  {
    Token& token = tokens[idx];
    if (token.GetType() == eTT_ParenClose)
    {
      break;
    }
    if (token.GetType() == eTT_Comma)
    {
      idx++;
      continue;
    }
    TypeDef def = GetType(tokenizer, idx);

    token = tokens[idx];
    if (token.GetType() == eTT_Comma || token.GetType() == eTT_ParenClose)
    {
      func->Add(Argument(def));
    }

    if (token.GetType() == eTT_Identifier)
    {
      std::string name = GetName(tokenizer, idx);
      func->Add(Argument(def, name));
    }

    token = tokens[idx];
    if (token.GetType() == eTT_Comma)
    {
      idx++;
    }
    else if (token.GetType() == eTT_ParenClose)
    {
      break;
    }
    else
    {
      int paren = 0;
      int angularBracket = 0;
      int curlyBrace = 0;
      int bracket = 0;
      for (size_t in = tokens.size(); idx < in; idx++)
      {
        Token& token = tokens[idx];
        switch (token.GetType())
        {
        case eTT_ParenOpen:
          parent++;
          break;
        case eTT_ParenClose:
          if (paren > 0)
          {
            parent--;
          }
          break;
        case eTT_AngleBracketOpen:
          angularBracket++;
          break;
        case eTT_AngleBracketClose:
          angularBracket--;
          break;
        case eTT_CurlyBraceOpen:
          curlyBrace++;
          break;
        case eTT_CurlyBraceClose:
          curlyBrace--;
          break;
        }
        if (token.GetType() == eTT_Comma || token.GetType() == eTT_ParenClose)
        {
          if (paren == 0 && angularBracket == 0 && curlyBrace == 0 && bracket == 0)
          {
            break;
          }
        }
      }
    }
  }

  for (size_t in = tokens.size(); idx < in;)
  {
    Token& token = tokens[idx++];
    if (token.GetType() == eTT_SemiColon)
    {
      break;
    }
    else if (token.GetType() == eTT_CurlyBraceOpen)
    {
      SkipBlock(tokenizer, idx);
      break;
    }
    else if (token.GetType() == eTT_Const)
    {
      func->SetConst(true);
    }
    else if (token.GetType() == eTT_Equal)
    {
      if (tokens[idx].Get() == "0")
      {
        func->SetPureVirtual(true);
      }
    }

  }


  return func;
}

FunctionNode* Parser::ParseFunction2(Tokenizer & tokenizer, size_t & idx, ASTNode * parent)
{
  auto tokens = tokenizer.GetTokens();
  size_t startIdx = idx;
  size_t parenIdx = tokenizer.Find(eTT_ParenOpen, idx);
  size_t typeIdx = parenIdx;
  std::string name = ReverseName(tokenizer, typeIdx);
  TypeDef type = ReverseType(tokenizer, typeIdx);

  FunctionNode* methodNode = new FunctionNode();
  methodNode->SetName(name);
  methodNode->SetReturnValue(type);

  if (tokenizer.Has(eTT_Virtual, startIdx, parenIdx))
  {
    methodNode->SetVirtual(true);
  }

  idx = tokenizer.Find(eTT_ParenClose, parenIdx);
  idx++;

  bool done = false;
  bool checkPureVirtual = false;
  for (size_t in = tokens.size(); idx < in && !done; ++idx)
  {

    Token& token = tokens[idx];
    switch (token.GetType())
    {
    case eTT_Const:
      methodNode->SetConst(true);
      break;
    case eTT_SemiColon:
      done = true;
      break;

    case eTT_Equal:
      checkPureVirtual = true;
      break;


    case eTT_CurlyBraceOpen:
      SkipBlock(tokenizer, idx);
      done = true;
      break;

    case eTT_OtherCode:
    case eTT_Identifier:
      if (token.Get() == "0")
      {
        if (checkPureVirtual)
        {
          methodNode->SetPureVirtual(true);
        }
      }
      break;
    }
  }


  return methodNode;
}


MemberNode* Parser::ParseMember(Tokenizer & tokenizer, size_t & idx, ASTNode * parent)
{
  auto tokens = tokenizer.GetTokens();
  size_t startIdx = idx;
  size_t semiColonIdx = tokenizer.Find(eTT_SemiColon, idx);
  size_t typeIdx = semiColonIdx;
  std::string name = ReverseName(tokenizer, typeIdx);
  TypeDef type = ReverseType(tokenizer, typeIdx);

  idx = semiColonIdx + 1;
  MemberNode* memberNode = new MemberNode();
  memberNode->SetName(name);
  memberNode->SetType(type);
  return memberNode;
}


void Parser::SkipBlock(Tokenizer & tokenizer, size_t & idx)
{
  auto tokens = tokenizer.GetTokens();
  for (size_t in = tokens.size(); idx < in;)
  {
    Token& token = tokens[idx];
    if (token.GetType() == eTT_CurlyBraceOpen)
    {
      SkipBlock(tokenizer, ++idx);
    }
    else
    {
      ++idx;
    }
  }
}

std::string Parser::GetName(Tokenizer & tokenizer, size_t & idx)
{
  std::string name;
  auto tokens = tokenizer.GetTokens();
  Token& token = tokens[idx++];

  if (token.GetType() == eTT_Identifier)
  {
    name += token.Get();
    while (true)
    {
      token = tokens[idx];
      if (token.GetType() == eTT_DoubleColon)
      {
        idx++;
        name += token.Get();
      }
      else
      {
        break;
      }
      token = tokens[idx];
      if (token.GetType() == eTT_Identifier)
      {
        idx++;
        name += token.Get();
      }
      else
      {
        break;
      }
    }
  }
  return name;
}


std::string Parser::ReverseName(Tokenizer & tokenizer, size_t & idx)
{
  std::string name;
  auto tokens = tokenizer.GetTokens();
  int angleBrackets = 0;
  while (true)
  {
    Token& token = tokens[--idx];
    if (token.GetType() == eTT_AngleBracketClose)
    {
      name = ">" + name;
      angleBrackets++;
    }
    else if (token.GetType() == eTT_AngleBracketOpen)
    {
      name = "<" + name;
      angleBrackets--;
    }

    if (angleBrackets == 0)
    {

      if (token.GetType() == eTT_Identifier)
      {
        name = token.Get() + name;
      }
      token = tokens[--idx];
      if (token.GetType() == eTT_DoubleColon)
      {
        name = "::" + name;
        continue;
      }

      break;
    }
    else
    {
      name = token.Get() + name;
    }
  }
  return name;
}


TypeDef Parser::GetType(Tokenizer & tokenizer, size_t & idx)
{
  auto tokens = tokenizer.GetTokens();
  TypeDef def;
  bool haveIdentifier = false;
  while (true)
  {
    Token& token = tokens[idx++];
    if (token.GetType() == eTT_Virtual)
    {
      continue;
    }
    if (token.GetType() == eTT_AngleBracketOpen)
    {
      while (idx < tokens.size())
      {
        TypeDef subType = GetType(tokenizer, idx);
        def.Add(subType);

        token = tokens[idx];
        if (token.GetType() == eTT_Comma)
        {
          idx++;
          continue;
        }
        else if (token.GetType() == eTT_AngleBracketClose)
        {
          idx++;
          break;
        }
      }
      continue;
    }

    if (token.GetType() == eTT_Identifier)
    {
      if (haveIdentifier)
      {
        --idx;
        break;
      }
      haveIdentifier = true;
      def.Add(token);
      while (true)
      {
        token = tokens[idx];
        if (token.GetType() == eTT_DoubleColon)
        {
          idx++;
          def.Add(token);
        }
        else
        {
          break;
        }
        token = tokens[idx];
        if (token.GetType() == eTT_Identifier)
        {
          idx++;
          def.Add(token);
        }
        else
        {
          break;
        }
      }
    }
    else if (token.GetType() == eTT_Const
      || token.GetType() == eTT_Ampersand
      || token.GetType() == eTT_Asterisk
      || token.GetType() == eTT_DoubleAsterisk)
    {
      def.Add(token);
    }
    else
    {
      idx--;
      break;
    }
  }

  return def;
}

TypeDef Parser::ReverseType(Tokenizer & tokenizer, size_t & idx)
{
  auto tokens = tokenizer.GetTokens();
  int angleBrackets = 0;
  TypeDef def;
  while (true)
  {
    Token& token = tokens[idx--];
    if (token.GetType() == eTT_AngleBracketClose)
    {
      while (idx > 0)
      {
        TypeDef subType = ReverseType(tokenizer, idx);
        def.AddFront(subType);

        token = tokens[idx];
        if (token.GetType() == eTT_Comma)
        {
          idx--;
          continue;
        }
        else if (token.GetType() == eTT_AngleBracketOpen)
        {
          idx--;
          break;
        }
      }
      continue;
    }

    if (token.GetType() == eTT_Identifier)
    {
      def.AddFront(token);
      while (true)
      {
        token = tokens[idx];
        if (token.GetType() == eTT_DoubleColon)
        {
          idx--;
          def.AddFront(token);
        }
        else
        {
          break;
        }
        token = tokens[idx];
        if (token.GetType() == eTT_Identifier)
        {
          idx--;
          def.AddFront(token);
        }
        else
        {
          break;
        }
      }
    }
    else if (token.GetType() == eTT_Const
      || token.GetType() == eTT_Ampersand
      || token.GetType() == eTT_Asterisk
      || token.GetType() == eTT_DoubleAsterisk)
    {
      def.AddFront(token);
    }
    else
    {
      idx++;
      break;
    }
  }

  return def;

}


}