
#include <cobalt/core/cobaltfile/cscobaltfile.hh>
#include <cobalt/core/cobaltfile/cscobaltfileentry.hh>
#include <cobalt/cstypes.hh>
#include <iostream>
#include <fstream>

csCobaltFile::InputBuffer::InputBuffer(std::ifstream &stream, csUInt32 bufferSize)
  : m_stream(stream)
  , m_row(1)
  , m_column(1)
{
  m_buffer = new csUInt8[bufferSize];
  m_bufferSize = bufferSize;
  m_bufferCapacity = 0;
  m_idx = 0;
}

csCobaltFile::InputBuffer::~InputBuffer()
{
  delete[] m_buffer;
}

bool csCobaltFile::InputBuffer::HasMore()
{
  if (m_idx >= m_bufferSize || m_idx >= m_bufferCapacity)
  {
    fetchData();
  }
  return m_idx < m_bufferCapacity;
}

csUInt8 csCobaltFile::InputBuffer::GetByte()
{
  csUInt8 ch = m_buffer[m_idx++];
  if (ch == '\n')
  {
    m_row++;
    m_column = 1;
  }
  else if (ch != '\r')
  {
    m_column++;
  }
  return ch;
}

csUInt32 csCobaltFile::InputBuffer::GetRow() const
{
  return m_row;
}

csUInt32 csCobaltFile::InputBuffer::GetColumn() const
{
  return m_column;
}

void csCobaltFile::InputBuffer::fetchData()
{
  m_stream.read((char*)m_buffer, m_bufferSize);
  if (m_stream)
  {
    m_bufferCapacity = m_bufferSize;
  }
  else
  {
    m_bufferCapacity = m_stream.gcount();
  }
  m_idx = 0;
}


csCobaltFile::csCobaltFile()
  : m_danglingChar(0)
  , m_root(new csCobaltFileEntry("<root>"))
{

}

static std::string tokenMap[] = {
  "Invalid",
  "Number",
  "String",
  "Semicolon",
  "Colon",
  "Comma",
  "Hash",
  "ParenOpen",
  "ParenClose",
  "CurlyBraceOpen",
  "CurlyBraceClose", 
};


bool csCobaltFile::Parse(const std::string &fileName)
{
  std::ifstream file;
  file.open(fileName);
  if (!file.is_open())
  {
    return false;
  }

  std::cout << "Successfully opened: " << fileName << std::endl;
  InputBuffer buffer (file, 32);

  csCobaltFileEntry *parent = m_root;
  csCobaltFileEntry *currentEntry = 0;
  enum State
  {
    eS_ExpectEntry,
    eS_ExpectAttribsOrChildrenOrClose,
    eS_ExpectAttrib,
    eS_ExpectEntryOrChildrenOrClose,
  };

  std::vector<Token> tokens;
  while (true)
  {
    Token token = GetToken(buffer);
    if (token.type == eTT_Invalid)
    {
      break;
    }
    tokens.push_back(token);
  }


  
  State state = eS_ExpectEntry;
  for (int tokenCounter=0; tokenCounter<tokens.size(); ++tokenCounter)
  {
    Token token = tokens[tokenCounter];

    switch (state)
    {
    case eS_ExpectEntry:
      if (token.type != eTT_String)
      {
        std::cout << "eS_ExpectEntry: Expected Token: " << tokenMap[eTT_String] << " but got token: " << tokenMap[token.type] << " @ " << token.row << ":" << token.column << std::endl;
        return false;
      }
      else 
      {
        csCobaltFileEntry *newEntry = new csCobaltFileEntry();
        newEntry->SetTagName(token.value);
        parent->AddChild(newEntry);
        currentEntry = newEntry;
        state = eS_ExpectAttribsOrChildrenOrClose;
      }
      break;



    case eS_ExpectAttribsOrChildrenOrClose:
      if (token.type == eTT_ParenOpen)
      {
        state = eS_ExpectAttrib;
      }
      else if (token.type == eTT_CurlyBraceOpen)
      {
        parent = currentEntry;
        currentEntry = 0;
        state = eS_ExpectEntry;
      }
      else if (token.type == eTT_CurlyBraceClose)
      {
        currentEntry = parent;
        if (!currentEntry)
        {
          std::cout << "eS_ExpectAttribsOrChildrenOrClose: Invalid stack depth @ " << token.row << ":" << token.column << std::endl;
          return false;
        }
        parent = currentEntry->GetParent();
      }
      else
      {
        std::cout << "eS_ExpectAttribsOrChildrenOrClose: Expected Tokens: [" << tokenMap[eTT_CurlyBraceOpen] << ", " << tokenMap[eTT_CurlyBraceClose] << ", " << tokenMap[eTT_ParenOpen] << token.column << " but got token: " << tokenMap[token.type] << " @ " << token.row << ":" << std::endl;
        return false;
      }
      break;

    case eS_ExpectAttrib:
      
      if (token.type == eTT_String || token.type == eTT_Number)
      {
        if (tokenCounter + 1 >= tokens.size())
        {
          std::cout << "eS_ExpectAttrib: Invalid token size @ " << token.row << ":" << token.column << std::endl;
          return false;
        }

        Token nextToken = tokens[tokenCounter + 1];
        if (nextToken.type != eTT_Colon)
        {
          currentEntry->AddAttribute(token.value);
        }
        else
        {
          if (tokenCounter + 2 >= tokens.size())
          {
            std::cout << "eS_ExpectAttrib: Invalid token size @ " << token.row << ":" << token.column << std::endl;
            return false;
          }

          nextToken = tokens[tokenCounter + 2];
          if (nextToken.type != eTT_String && nextToken.type != eTT_Number)
          {
            std::cout << "eS_ExpectAttrib: Expected Tokens: " << tokenMap[eTT_String] << ", " << tokenMap[eTT_Number] << " but got " << tokenMap[token.type] << " @ " << token.row << ":" << token.column << std::endl;
            return false;
          }
          currentEntry->AddAttribute(token.value, nextToken.value);
          tokenCounter += 2;
        }
      }
      else if (token.type == eTT_ParenClose)
      {
        state = eS_ExpectEntryOrChildrenOrClose;
      }
      else
      {
        std::cout << "eS_ExpectAttrib: Expected Tokens: " << tokenMap[eTT_String] << ", " << tokenMap[eTT_Number] << ", " << tokenMap[eTT_ParenClose] << " but got " << tokenMap[token.type] << " @ " << token.row << ":" << token.column << std::endl;
        return false;
      }
      break;

    case eS_ExpectEntryOrChildrenOrClose:
      if (token.type == eTT_CurlyBraceOpen)
      {
        parent = currentEntry;
        currentEntry = 0;
        state = eS_ExpectEntry;
      }
      else if (token.type == eTT_CurlyBraceClose)
      {
        currentEntry = parent;
        if (!currentEntry)
        {
          std::cout << "eS_ExpectEntryOrChildrenOrClose: Invalid stack depth " << " @ " << token.row << ":" << token.column << std::endl;
          return false;
        }
        parent = currentEntry->GetParent();
      }
      else if (token.type == eTT_String)
      {
        tokenCounter--;
        state = eS_ExpectEntry;
      }
      else
      {
        std::cout << "eS_ExpectEntryOrChildrenOrClose: Expected Tokens: [" << tokenMap[eTT_CurlyBraceOpen] << ", " << tokenMap[eTT_CurlyBraceClose] << ", " << tokenMap[eTT_String] << " @ " << token.row << ":" << token.column << " but got token: " << tokenMap[token.type] << std::endl;
        return false;
      }
      break;


    }

  }

  m_root->Debug();

  return true;
}

csCobaltFile::Token csCobaltFile::GetToken(csCobaltFile::InputBuffer& buffer)
{
  std::string token = "";
  std::string fullToken = "";
  if (m_danglingChar)
  {
    token += m_danglingChar;
    fullToken += m_danglingChar;
    m_danglingChar = 0;
  }
  csUInt32 row = buffer.GetRow();
  csUInt32 column = buffer.GetColumn();
  while (buffer.HasMore())
  {
    char ch = (char)buffer.GetByte();
    Token t = CheckToken(token + ch, fullToken + ch, row, column);
    if (t.type != eTT_Invalid)
    {
      // valid token
      token += ch;
      fullToken += ch;
      continue;
    }
    t = CheckToken(token, fullToken, row, column);
    if (t.type != eTT_Invalid)
    {
      if (!iswspace(ch)) 
      {
        m_danglingChar = ch;
      }
      return t;
    }

    if (!iswspace(ch))
    {
      token += ch;
    }
    fullToken += ch;
  }
  
  return CheckToken(token, fullToken, row, column);
}

csCobaltFile::Token csCobaltFile::CheckToken(const std::string &token, const std::string &fullToken, csUInt32 row, csUInt32 column) const
{
  Token result;
  result.value = token;
  result.type = eTT_Invalid;
  result.row = row;
  result.column = column;
  if (token.empty())
  {
    return result;
  }
  if (token.length() == 1)
  {
    if (token[0] == '{')
    {
      result.type = eTT_CurlyBraceOpen;
    }
    else if (token[0] == '}')
    {
      result.type = eTT_CurlyBraceClose;
    }
    if (token[0] == '(')
    {
      result.type = eTT_ParenOpen;
    }
    else if (token[0] == ')')
    {
      result.type = eTT_ParenClose;
    }
    if (token[0] == ',')
    {
      result.type = eTT_Comma;
    }
    else if (token[0] == ';')
    {
      result.type = eTT_Semicolon;
    }
    else if (token[0] == ':')
    {
      result.type = eTT_Colon;
    }
    else if (token[0] == '#')
    {
      result.type = eTT_Colon;
    }
  }
  if (result.type == eTT_Invalid)
  {

    if (IsNumber(token))
    {
      result.type = eTT_Number;
    }
    else if (IsString(token))
    {
      result.value = FixString(fullToken);
      result.type = eTT_String;
    }
  }
  return result;
}


bool csCobaltFile::IsNumber(const std::string &token) const
{
  bool haveDot = false;
  bool haveE = false;
  for (size_t i = 0, in = token.length(); i < in; ++i)
  {
    char ch = token[i];
    if (!isdigit(ch))
    {
      if (ch == 'e' || ch == 'E')
      {
        if (i == 0 || !isdigit(token[i-1]) || haveE)
        {
          return false;
        }
        haveE = true;
      }
      else if (ch == '-' || ch == '+')
      {
        if (i != 0 && token[i - 1] != 'e' && token[i - 1] != 'E')
        {
          return false;
        }
      }
      else if (ch == '.')
      {
        if (haveDot)
        {
          return false;
        }
        haveDot = true;
      }
      else 
      {
        return false;
      }
    }
  }

  return true;
}

bool csCobaltFile::IsString(const std::string &token) const
{
  if (token.empty())
  {
    return false;
  }

  
  if (token[0] == '"')
  {
    for (size_t i = 0, in = token.length(); i < in; ++i)
    {
      char ch = token[i];
      if (ch == '\\')
      {
        i++;
      }
      else if (ch == '"' && i != 0)
      {
        if (i + 1 == in)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
    }
    return false;
  }
  else
  {

    for (size_t i = 0, in = token.length(); i < in; ++i)
    {
      char ch = token[i];
      if (iswspace(ch))
      {
        return false;
      }
      if (isdigit(ch) && i == 0)
      {
        return false;
      }
      if (!isalpha(ch) && ch != '_')
      {
        return false;
      }
    }
  }
  return true;
}


std::string csCobaltFile::FixString(const std::string &str) const
{
  std::string result = "";
  bool stripWhiteSpace = true;
  for (unsigned i = 0, in = str.length(); i < in; ++i)
  {
    char ch = str[i];
    if (iswspace(ch))
    {
      if (stripWhiteSpace)
      {
        continue;
      }
    }
    else
    {
      stripWhiteSpace = false;
    }
    if (ch == '"')
    {
      continue;
    }

    if (ch == '\\')
    {
      if (i + 1 < in)
      {
        result += str[++i];
      }
      continue;
    }

    result += ch;
  }
  return result;
}

