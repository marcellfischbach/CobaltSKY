
#pragma once

#include <parser/token.hh>
#include <string>
#include <vector>

namespace cs::classgenerator
{


class SourceFile;
class Tokenizer
{
public:
  Tokenizer(const SourceFile& sourceFile);

  const std::vector<Token>& GetTokens() const;

private:
  bool IsIdentifier(const std::string& token) const;
  
  std::vector<Token> m_tokens;
};


}