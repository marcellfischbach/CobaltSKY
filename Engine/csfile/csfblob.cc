
#include <csfile/csfblob.hh>
#include <csfile/csffile.hh>


cs::file::Blob::Blob(cs::file::File *file)
  : m_file(file)
  , m_name("")
  , m_buffer(0)
  , m_size(0)
{

}


cs::file::Blob::~Blob()
{
  m_file = 0;
  if (m_buffer)
  {
    delete[] m_buffer;
    m_buffer = 0;
  }
}


cs::file::File *cs::file::Blob::GetFile()
{
  return m_file;
}

const cs::file::File *cs::file::Blob::GetFile() const
{
  return m_file;
}

void cs::file::Blob::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &cs::file::Blob::GetName() const
{
  return m_name;
}

void cs::file::Blob::SetType(const std::string &type)
{
  m_type = type;
}

const std::string &cs::file::Blob::GetType() const
{
  return m_type;
}

void cs::file::Blob::SetBuffer(const void *buffer, size_t size)
{
  if (m_buffer)
  {
    delete[] m_buffer;
    m_buffer = 0;
  }

  if (buffer && size)
  {
    m_buffer = new csUInt8[size];
    memcpy(m_buffer, buffer, size);
    m_size = size;
  }
}

const void *cs::file::Blob::GetBuffer() const
{
  return m_buffer;
}

size_t cs::file::Blob::GetSize() const
{
  return m_size;
}
