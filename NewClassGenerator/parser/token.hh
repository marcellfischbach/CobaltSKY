#pragma once


#include <string>

namespace cs::classgenerator
{

enum TokenType
{
  eTT_Identifier,
  eTT_Class,
  eTT_Namespace,
  eTT_CurlyBraceOpen,
  eTT_CurlyBraceClose,
  eTT_Colon,
  eTT_DoubleColon,
  eTT_SemiColon,
  eTT_Equal,
  eTT_OtherCode,
};

class Token
{
public:
  Token(const std::string& token = "");

  const std::string& Get() const
  {
    return m_token;
  }

  TokenType GetType() const
  {
    return m_type;
  }


private:
  bool IsIdentifier();
  
  TokenType m_type;
  std::string m_token;
};


}