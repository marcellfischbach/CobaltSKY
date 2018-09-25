#pragma once

#include <csfile/csfexport.hh>
#include <cobalt/cstypes.hh>
#include <string>

class csfFile;

class CSF_API csfBlob
{
  friend class csfFile;
public:
  ~csfBlob();

  csfFile *GetFile();
  const csfFile *GetFile() const;

  void SetName(const std::string &name);
  const std::string &GetName() const;

  void SetType(const std::string &type);
  const std::string &GetType() const;

  void SetBuffer(const void *buffer, size_t size);
  const void *GetBuffer() const;
  size_t GetSize() const;

private:
  csfBlob(csfFile *file);

  csfFile *m_file;
  std::string m_name;
  std::string m_type;
   csUInt8 *m_buffer;
  size_t m_size;
};
