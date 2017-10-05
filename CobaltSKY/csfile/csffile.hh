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

  bool Parse(const csUInt8* buffer, size_t size);
  bool Parse (const std::string &fileName);
  bool Output (const std::string &fileName, bool tight = false, unsigned indent = 2) const;

  csfEntry *GetRoot();
  const csfEntry *GetRoot() const;

  const csfEntry *GetEntry(const std::string &entry) const;

  const csfBlob *GetBlob(const std::string &blob) const;

  void AddBlob(csfBlob *blob);

  csfEntry *CreateEntry(const std::string &tagName = std::string(""));
  csfBlob *CreateBlob();

  bool IsError() const;
  const std::string &GetErrorMessage() const;

private:
  struct IInputBuffer
  {
    virtual ~IInputBuffer() { }
    virtual bool HasMore() = 0;
    virtual csUInt8 GetByte() = 0;
    virtual csUInt32 GetRow() const = 0;
    virtual csUInt32 GetColumn() const = 0;
  };


  class InputStreamBuffer : public IInputBuffer
  {
  public:
    InputStreamBuffer(std::ifstream &stream, csUInt32 bufferSize = 1024);
    virtual ~InputStreamBuffer();
    bool HasMore();
    csUInt8 GetByte();
    csUInt32 GetRow() const;
    csUInt32 GetColumn() const;
  private:
    void fetchData();
    std::ifstream &m_stream;

    csUInt8 *m_buffer;
    csUInt32 m_idx;
    csUInt32 m_bufferSize;
    csUInt32 m_bufferCapacity;
    csUInt32 m_row;
    csUInt32 m_column;
  };

  class InputDataBuffer : public IInputBuffer
  {
  public:
    InputDataBuffer(const csUInt8 *buffer, size_t size);
    virtual ~InputDataBuffer();
    bool HasMore();
    csUInt8 GetByte();
    csUInt32 GetRow() const;
    csUInt32 GetColumn() const;
  private:
    const csUInt8 *m_buffer;
    size_t m_bufferSize;
    csUInt32 m_idx;
    csUInt32 m_row;
    csUInt32 m_column;
  };

  bool Parse(IInputBuffer *buffer);

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

  Token GetToken(IInputBuffer *buffer);
  Token CheckToken(const std::string &token, const std::string &fullToken, csUInt32 row, csUInt32 column) const;
  bool IsNumber(const std::string &token) const;
  bool IsString(const std::string &token) const;
  std::string FixString(const std::string &token) const;
  csfBlob *GetBlob(IInputBuffer *buffer);

  void Output(csfEntry *entry, std::ofstream &ofstream, bool tight, unsigned indent, unsigned currentIndent) const;
  void Output(csfBlob *blob, std::ofstream &ofstream, bool tight) const;
  std::string Indent(unsigned indent) const;
  bool NeedQuotation(const std::string &value) const;
  bool IsUnquotedChar(char ch) const;

  bool MapError(const std::string &errorMessage) const;

  char m_danglingChar;

  csfEntry *m_root;
  std::vector<csfBlob*> m_blobs;

  mutable bool m_error;
  mutable std::string m_errorMessage;
};

