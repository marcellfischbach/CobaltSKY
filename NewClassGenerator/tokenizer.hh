
#pragma once


#include <string>
#include <vector>


namespace cs::classgenerator
{

class SourceFile;
class Tokenizer
{
public:
  Tokenizer(const SourceFile& sourceFile);

  const std::vector<std::string>& GetTokens() const;

private:
  
  std::vector<std::string> m_tokens;
};


}