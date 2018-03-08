
#include "tokenizer.hh"



Tokenizer::Tokenizer(const std::string &line)
{

  m_tokens.clear();
  std::string token = "";

  for (size_t i = 0, n = line.length(); i < n; ++i)
  {
    char ch = line[i];

    if (ch >= 'a' && ch <= 'z' ||
        ch >= 'A' && ch <= 'Z' ||
        ch >= '0' && ch <= '9' ||
        ch == '_' || ch == '.')
    {
      token += ch;
    }
    else if (ch == ' ' || ch == '\t')
    {
      // whitespace this starts a new token
      if (token.length() != 0)
      {
        m_tokens.push_back(token);
        token = "";
      }
    }
    else if (ch == ':')
    {
      if (i + 1 < line.length() && line[i+1] == ':')
      {
        token += "::";
        ++i;
      }
      else
      {
        if (token.length() != 0)
        {
          m_tokens.push_back(token);
          token = "";
        }
        token += ch;
        // for every other item we make a new token
        m_tokens.push_back(token);
        token = "";

      }
    }
    else if (ch == '*')
    {
      if (token.length() != 0)
      {
        m_tokens.push_back(token);
        token = "";
      }

      if (i + 1 < line.length() && line[i + 1] == '*')
      {
        token = "**";
        ++i;
      }
      else
      {
        token = "*";
      }

      m_tokens.push_back(token);
      token = "";
    }
    else if (ch == '<' || ch =='>' || ch == ',')
    {
      if (token.length() != 0)
      {
        m_tokens.push_back(token);
        token = "";
      }
      token += ch;
      m_tokens.push_back(token);
      token = "";
    }
    else 
    {
      if (token.length() != 0)
      {
        m_tokens.push_back(token);
        token = "";
      }
      token += ch;
      // for every other item we make a new token
      m_tokens.push_back(token);
      token = "";
    }
  }
  if (token.length() != 0)
  {
    m_tokens.push_back(token);
  }
}


size_t Tokenizer::GetNumberOfTokens() const
{
  return m_tokens.size();
}

const std::string &Tokenizer::GetToken(size_t idx) const
{
  return m_tokens[idx];
}