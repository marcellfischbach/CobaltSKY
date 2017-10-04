#pragma once

#include <csfile/csfexport.hh>
#include <csfile/csfblob.hh>
#include <csfile/csfentry.hh>
#include <cobalt/cstypes.hh>
#include <string>
#include <vector>

class CSF_API csfFile
{
public:
  csfFile();

  bool Parse (const std::string &fileName);
  bool Output (const std::string &fileName, bool tight = false, unsigned indent = 2) const;

  csfEntry *GetRoot();
  const csfEntry *GetRoot() const;

  const csfEntry *GetEntry(const std::string &entry) const;

  const csfBlob *GetBlob(const std::string &blob) const;

  void AddBlob(csfBlob *blob);

  csfEntry *CreateEntry();
  csfBlob *CreateBlob();

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
  csfBlob *GetBlob(InputBuffer &buffer);

  void Output(csfEntry *entry, std::ofstream &ofstream, bool tight, unsigned indent, unsigned currentIndent) const;
  void Output(csfBlob *blob, std::ofstream &ofstream, bool tight) const;
  std::string Indent(unsigned indent) const;
  bool NeedQuotation(const std::string &value) const;

  char m_danglingChar;

  csfEntry *m_root;
  std::vector<csfBlob*> m_blobs;
};

