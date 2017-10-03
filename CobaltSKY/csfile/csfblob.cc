
#include <csfile/csfblob.hh>
#include <csfile/csffile.hh>


csfBlob::csfBlob(csfFile *file)
  : m_file(file)
  , m_name("")
  , m_buffer(0)
  , m_size(0)
{

}


csfBlob::~csfBlob()
{
  m_file = 0;
  if (m_buffer)
  {
    delete[] m_buffer;
    m_buffer = 0;
  }
}


csfFile *csfBlob::GetFile()
{
  return m_file;
}

const csfFile *csfBlob::GetFile() const
{
  return m_file;
}

void csfBlob::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &csfBlob::GetName() const
{
  return m_name;
}


void csfBlob::SetBuffer(const void *buffer, size_t size)
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

const void *csfBlob::GetBuffer() const
{
  return m_buffer;
}

size_t csfBlob::GetSize() const
{
  return m_size;
}
