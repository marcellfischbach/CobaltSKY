#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <string>

class csCobaltFileEntry;
class CSE_API csCobaltFile
{
public:
  csCobaltFile();

  bool Parse (const std::string &fileName);

private:
  class InputBuffer
  {
  public:
    InputBuffer(std::ifstream &stream, csUInt32 bufferSize = 1024);
    ~InputBuffer();
    bool HasMore();
    csUInt8 GetByte();
    csUInt32 GetRow() const;
    csUInt32 GetColumn() const;
    void fetchData();
  private:
    std::ifstream &m_stream;

    csUInt8 *m_buffer;
    csUInt32 m_idx;
    csUInt32 m_bufferSize;
    csUInt32 m_bufferCapacity;
    csUInt32 m_row;
    csUInt32 m_column;
  };

  enum TokenType
  {
    eTT_Invalid,
    eTT_Number,
    eTT_String,
    eTT_Semicolon,
    eTT_Colon,
    eTT_Comma,
    eTT_Hash,
    eTT_ParenOpen,
    eTT_ParenClose,
    eTT_CurlyBraceOpen,
    eTT_CurlyBraceClose,
  };

  struct Token
  {
    TokenType type;
    std::string value;
    csUInt32 row;
    csUInt32 column;
  };

  Token GetToken(InputBuffer &buffer);
  Token CheckToken(const std::string &token, const std::string &fullToken, csUInt32 row, csUInt32 column) const;
  bool IsNumber(const std::string &token) const;
  bool IsString(const std::string &token) const;
  std::string FixString(const std::string &token) const;


  char m_danglingChar;

  csCobaltFileEntry *m_root;
};

