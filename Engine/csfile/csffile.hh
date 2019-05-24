#pragma once

#include <csfile/csfexport.hh>
#include <csfile/csfblob.hh>
#include <csfile/csfentry.hh>
#include <cobalt/cstypes.hh>
#include <string>
#include <vector>

namespace cs::file
{

class CSF_API File
{
public:
  File();
  ~File();

  bool Parse(const csUInt8* buffer, size_t size, bool parseBinarySection = true);
  bool Parse(const std::string& fileName, bool parseBinarySection = true);
  bool Output(const std::string& fileName, bool tight = false, unsigned indent = 2) const;

  cs::file::Entry* GetRoot();
  const cs::file::Entry* GetRoot() const;
  cs::file::Entry* GetEntry(const std::string& entry);
  const cs::file::Entry* GetEntry(const std::string& entry) const;
  cs::file::Blob* GetBlob(const std::string& blob);
  const cs::file::Blob* GetBlob(const std::string& blob) const;

  void AddBlob(cs::file::Blob* blob);

  cs::file::Entry* CreateEntry(const std::string& tagName = std::string(""));
  cs::file::Blob* CreateBlob();

  bool IsError() const;
  const std::string& GetErrorMessage() const;

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
    InputStreamBuffer(std::ifstream& stream, size_t bufferSize = 1024);
    virtual ~InputStreamBuffer();
    bool HasMore();
    csUInt8 GetByte();
    csUInt32 GetRow() const;
    csUInt32 GetColumn() const;
  private:
    void fetchData();
    std::ifstream& m_stream;

    csUInt8* m_buffer;
    csUInt32 m_idx;
    size_t m_bufferSize;
    size_t m_bufferCapacity;
    csUInt32 m_row;
    csUInt32 m_column;
  };

  class InputDataBuffer : public IInputBuffer
  {
  public:
    InputDataBuffer(const csUInt8* buffer, size_t size);
    virtual ~InputDataBuffer();
    bool HasMore();
    csUInt8 GetByte();
    csUInt32 GetRow() const;
    csUInt32 GetColumn() const;
  private:
    const csUInt8* m_buffer;
    size_t m_bufferSize;
    csUInt32 m_idx;
    csUInt32 m_row;
    csUInt32 m_column;
  };

  bool Parse(IInputBuffer* buffer, bool parseBinarySection);

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
    TokenType type = eTT_Invalid;
    std::string value;
    csUInt32 row = 0;
    csUInt32 column = 0;
  };

  Token GetToken(IInputBuffer* buffer);
  Token CheckToken(const std::string& token, const std::string& fullToken, csUInt32 row, csUInt32 column) const;
  bool IsNumber(const std::string& token) const;
  bool IsString(const std::string& token) const;
  std::string FixString(const std::string& token) const;
  cs::file::Blob* GetBlob(IInputBuffer* buffer);

  void Output(cs::file::Entry* entry, std::ofstream& ofstream, bool tight, unsigned indent, unsigned currentIndent) const;
  void Output(cs::file::Blob* blob, std::ofstream& ofstream, bool tight) const;
  std::string Indent(unsigned indent) const;
  bool NeedQuotation(const std::string& value) const;
  bool IsUnquotedChar(char ch) const;

  bool MapError(const std::string& errorMessage) const;

  char m_danglingChar;

  cs::file::Entry* m_root;
  std::vector<cs::file::Blob*> m_blobs;
  std::vector<cs::file::Entry*> m_entries;

  mutable bool m_error;
  mutable std::string m_errorMessage;
};


}

