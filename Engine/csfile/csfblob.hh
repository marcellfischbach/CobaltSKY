#pragma once

#include <csfile/csfexport.hh>
#include <cobalt/cstypes.hh>
#include <string>


namespace cs::file
{

class File;

class CSF_API Blob
{
  friend class File;
public:
  ~Blob();

  cs::file::File* GetFile();
  const cs::file::File* GetFile() const;

  void SetName(const std::string& name);
  const std::string& GetName() const;

  void SetType(const std::string& type);
  const std::string& GetType() const;

  void SetBuffer(const void* buffer, size_t size);
  const void* GetBuffer() const;
  size_t GetSize() const;

private:
  Blob(cs::file::File* file);

  cs::file::File* m_file;
  std::string m_name;
  std::string m_type;
  csUInt8* m_buffer;
  size_t m_size;
};

}

