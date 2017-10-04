
#include <csfile/csffile.hh>
#include <csfile/csfblob.hh>
#include <csfile/csfentry.hh>
#include <cobalt/cstypes.hh>
#include <iostream>
#include <fstream>

csfFile::InputBuffer::InputBuffer(std::ifstream &stream, csUInt32 bufferSize)
  : m_stream(stream)
  , m_row(1)
  , m_column(1)
{
  m_buffer = new csUInt8[bufferSize];
  m_bufferSize = bufferSize;
  m_bufferCapacity = 0;
  m_idx = 0;
}

csfFile::InputBuffer::~InputBuffer()
{
  delete[] m_buffer;
}

bool csfFile::InputBuffer::HasMore()
{
  if (m_idx >= m_bufferSize || m_idx >= m_bufferCapacity)
  {
    fetchData();
  }
  return m_idx < m_bufferCapacity;
}

csUInt8 csfFile::InputBuffer::GetByte()
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

csUInt32 csfFile::InputBuffer::GetRow() const
{
  return m_row;
}

csUInt32 csfFile::InputBuffer::GetColumn() const
{
  return m_column;
}

void csfFile::InputBuffer::fetchData()
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


csfFile::csfFile()
  : m_danglingChar(0)
  , m_root(new csfEntry(this, "<root>"))
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


bool csfFile::Parse(const std::string &fileName)
{
  std::ifstream file;
  file.open(fileName);
  if (!file.is_open())
  {
    std::cout << "Unable to open file: " << fileName << std::endl;
    return false;
  }

  std::cout << "Successfully opened: " << fileName << std::endl;
  InputBuffer buffer (file, 32);

  csfEntry *parent = m_root;
  csfEntry *currentEntry = 0;
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
    if (token.type == eTT_Hash)
    {
      csfBlob *blob = GetBlob(buffer);
      AddBlob(blob);
      continue;
    }
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
        std::cout << "eS_ExpectEntry: Expected Token: " << tokenMap[eTT_String] << ", " << tokenMap[eTT_Hash] <<  " but got token: " << tokenMap[token.type] << " @ " << token.row << ":" << token.column << std::endl;
        return false;
      }
      else 
      {
        csfEntry *newEntry = new csfEntry(this);
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

csfFile::Token csfFile::GetToken(csfFile::InputBuffer& buffer)
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

csfBlob *csfFile::GetBlob(csfFile::InputBuffer &buffer)
{
  Token token = GetToken(buffer);
  if (token.type != eTT_String)
  {
    std::cout << "Misformed blob: Expected '" << tokenMap[eTT_String] << "' but got " << tokenMap[token.type] << " @ " << token.row << ":" << token.column << std::endl;
    return 0;
  }
  std::string blobName = token.value;


  token = GetToken(buffer);
  if (token.type != eTT_Colon)
  {
    std::cout << "Misformed blob: Expected '" << tokenMap[eTT_Colon] << "' but got " << tokenMap[token.type] << " @ " << token.row << ":" << token.column << std::endl;
    return 0;
  }

  csUInt8 b[4];
  b[0] = m_danglingChar; m_danglingChar = 0;
  for (unsigned i = 1; i < 4; ++i)
  {
    if (!buffer.HasMore())
    {
      std::cout << "Misformed blob: Expected buffer size  @ " << token.row << ":" << token.column << std::endl;
      return 0;
    }
    b[i] = buffer.GetByte();
  }

  csUInt32 size = *reinterpret_cast<csUInt32*>(b);
  csUInt8 *blobBuffer = new csUInt8[size];
  for (csUInt32 i = 0; i < size; ++i)
  {
    if (!buffer.HasMore())
    {
      delete[] blobBuffer;
      std::cout << "Misformed blob: Expected buffer size  " << size << " @ " << token.row << ":" << token.column << std::endl;
      return 0;
    }
    blobBuffer[i] = buffer.GetByte();
  }

  csfBlob *blob = CreateBlob();
  blob->SetName(blobName);
  blob->SetBuffer(blobBuffer, size);
  delete[] blobBuffer;

  return blob;
}

csfFile::Token csfFile::CheckToken(const std::string &token, const std::string &fullToken, csUInt32 row, csUInt32 column) const
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
      result.type = eTT_Hash;
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


bool csfFile::IsNumber(const std::string &token) const
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

bool csfFile::IsString(const std::string &token) const
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
      if (!isalpha(ch) && ch != '_' && ch != '.')
      {
        return false;
      }
    }
  }
  return true;
}


std::string csfFile::FixString(const std::string &str) const
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

csfEntry *csfFile::GetRoot()
{
  return m_root;
}

const csfEntry *csfFile::GetRoot() const
{
  return m_root;
}

const csfEntry *csfFile::GetEntry(const std::string &entry) const
{
  return m_root->GetEntry(entry);
}

const csfBlob *csfFile::GetBlob(const std::string &blob) const
{
  for (const csfBlob *b : m_blobs)
  {
    if (b->GetName() == blob)
    {
      return b;
    }
  }
  return 0;
}

csfEntry *csfFile::CreateEntry()
{
  return new csfEntry(this);
}

csfBlob *csfFile::CreateBlob()
{
  return new csfBlob(this);
}

void csfFile::AddBlob(csfBlob *blob)
{
  if (!blob)
  {
    return;
  }
  for (csfBlob *b : m_blobs)
  {
    if (b == blob || b->GetName() == blob->GetName())
    {
      return;
    }
  }

  m_blobs.push_back(blob);
}


bool csfFile::Output(const std::string &fileName, bool tight, unsigned indent) const
{
  std::ofstream ofstream;
  ofstream.open(fileName, std::ofstream::out | std::ofstream::trunc);
  if (!ofstream.is_open())
  {
    std::cout << "Unable to open file '" << fileName << "' for writing" << std::endl;
    return false;
  }


  for (size_t i = 0, in = m_root->GetNumberOfChildren(); i < in; ++i)
  {
    csfEntry *child = m_root->GetChild(i);
    if (child)
    {
      Output(child, ofstream, tight, indent, 0);
    }
  }

  for (csfBlob *blob : m_blobs)
  {
    Output(blob, ofstream, tight);
  }
  ofstream.close();
  return true;
}

void csfFile::Output(csfEntry *entry, std::ofstream &ofstream, bool tight, unsigned indent, unsigned currentIndent) const
{
  if (!tight)
  {
    ofstream << Indent(currentIndent);
  }
  ofstream << entry->GetTagName();

  if (entry->GetNumberOfAttributes() > 0)
  {
    if (!tight)
    {
      ofstream << " ";
    }

    ofstream << "(";
    for (size_t i = 0, in = entry->GetNumberOfAttributes(); i < in; ++i)
    {
      std::string key = entry->GetAttributeKey(i);
      std::string value = entry->GetAttributeValue(i);
      if (!key.empty())
      {
        ofstream << key << ":";
      }
      bool needQuotation = NeedQuotation(value);
      if (needQuotation)
      {
        ofstream << "\"";
      }
      ofstream << value;
      if (needQuotation)
      {
        ofstream << "\"";
      }
      if (i + 1 != in)
      {
        ofstream << " ";
      }
    }
    ofstream << ")";
  }
  if (entry->GetNumberOfChildren() > 0)
  {
    if (!tight)
    {
      ofstream << std::endl << Indent(currentIndent) << "{" << std::endl;
    }
    else
    {
      ofstream << "{";
    }
    for (size_t i = 0, in = entry->GetNumberOfChildren(); i < in; ++i)
    {
      csfEntry *child = entry->GetChild(i);
      if (child)
      {
        Output(child, ofstream, tight, indent, currentIndent + indent);
      }
    }
    if (!tight)
    {
      ofstream << Indent(currentIndent);
    }
    ofstream << "}";
  }
  if (!tight)
  {
    ofstream << std::endl;
  }

}

void csfFile::Output(csfBlob *blob, std::ofstream &ofstream, bool tight) const
{
  ofstream << "#" << blob->GetName() << ":";
  csUInt32 size = blob->GetSize();
  ofstream.write((const char*)&size, sizeof(size));
  ofstream.write((const char*)blob->GetBuffer(), blob->GetSize());
  if (!tight)
  {
    ofstream << std::endl;
  }
}

std::string csfFile::Indent(unsigned indent) const
{
  std::string result;
  for (unsigned i = 0; i < indent; ++i)
  {
    result += " ";
  }
  return result;
}

bool csfFile::NeedQuotation(const std::string &value) const
{
  if (IsNumber(value))
  {
    return false;
  }
  for (size_t i = 0, in = value.length(); i < in; ++i)
  {
    char ch = value[i];
    if (isalpha(ch))
    {
      continue;
    }
    return true;
  }
  return false;
}