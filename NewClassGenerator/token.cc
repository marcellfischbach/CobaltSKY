

#include "token.hh"


namespace cs::classgenerator
{

#define CHECK_TKN(tkn, tknEnum) if (m_token == tkn)  m_type = tknEnum
#define E_CHECK_TKN(tkn, tknEnum) else if (m_token == tkn)  m_type = tknEnum

Token::Token(const std::string & token)
  : m_token(token)
{
  CHECK_TKN("class", eTT_Class);
  E_CHECK_TKN("namespace", eTT_Namespace);
  E_CHECK_TKN("{", eTT_CurlyBraceOpen);
  E_CHECK_TKN("}", eTT_CurlyBraceClose);
  E_CHECK_TKN(":", eTT_Colon);
  E_CHECK_TKN("::", eTT_DoubleColon);
  E_CHECK_TKN(";", eTT_SemiColon);
  E_CHECK_TKN("=", eTT_Equal);
  else if (IsIdentifier())
  {
    m_type = eTT_Identifier;
  }
  else
  {
    m_type = eTT_OtherCode;
  }

}

bool Token::IsIdentifier()
{

  if (m_token.size() == 0)
  {
    return false;
  }

  char ch = m_token[0];
  if (!(::isalpha(ch) || ch == '_'))
  {
    return false;
  }


  for (size_t i = 1, in = m_token.size(); i < in; ++i)
  {
    ch = m_token[0];
    if (!(::isalnum(ch) || ch == '_'))
    {
      return false;
    }
  }
  return true;
}


}