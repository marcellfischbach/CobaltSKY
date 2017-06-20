

#include <cobalt/core/csassetinputstream.hh>


csAssetInputStream::csAssetInputStream(const csUInt8 *buffer, csSize bufferSize)
  : m_readPointer(buffer)
  , m_buffer(buffer)
  , m_bufferSize(bufferSize)
{

}

csAssetInputStream::~csAssetInputStream()
{
  m_buffer = 0;
  m_readPointer = 0;
}


csAssetInputStream &csAssetInputStream::operator>>(csInt8 &o)
{
  o = *reinterpret_cast<const csInt8*>(m_readPointer);
  m_readPointer++;
  return *this;
}


csAssetInputStream &csAssetInputStream::operator>>(csUInt8 &o)
{
  o = *reinterpret_cast<const csUInt8*>(m_readPointer);
  m_readPointer++;
  return *this;
}



csAssetInputStream &csAssetInputStream::operator>>(csInt16 &o)
{
  o = *reinterpret_cast<const csInt16*>(m_readPointer);
  m_readPointer += sizeof(csInt16);
  return *this;
}


csAssetInputStream &csAssetInputStream::operator>>(csUInt16 &o)
{
  o = *reinterpret_cast<const csUInt16*>(m_readPointer);
  m_readPointer += sizeof(csUInt16);
  return *this;
}


csAssetInputStream &csAssetInputStream::operator>>(csInt32 &o)
{
  o = *reinterpret_cast<const csInt32*>(m_readPointer);
  m_readPointer += sizeof(csInt32);
  return *this;
}


csAssetInputStream &csAssetInputStream::operator>>(csUInt32 &o)
{
  o = *reinterpret_cast<const csUInt32*>(m_readPointer);
  m_readPointer += sizeof(csUInt32);
  return *this;
}


csAssetInputStream &csAssetInputStream::operator>>(csSize &o)
{
  o = *reinterpret_cast<const csSize*>(m_readPointer);
  m_readPointer += sizeof(csSize);
  return *this;
}



csAssetInputStream &csAssetInputStream::operator>>(float &o)
{
  o = *reinterpret_cast<const float*>(m_readPointer);
  m_readPointer += sizeof(float);
  return *this;
}



csAssetInputStream &csAssetInputStream::operator>>(csVector2f &o)
{
  *this >> o.x >> o.y;
  return *this;
}

csAssetInputStream &csAssetInputStream::operator>>(csVector3f &o)
{
  *this >> o.x >> o.y >> o.z;
  return *this;
}

csAssetInputStream &csAssetInputStream::operator>>(csVector4f &o)
{
  *this >> o.x >> o.y >> o.z >> o.w;
  return *this;
}


csAssetInputStream &csAssetInputStream::operator>>(csColor4f &o)
{
  *this >> o.r >> o.g >> o.b >> o.a;
  return *this;
}

csAssetInputStream &csAssetInputStream::operator>>(csMatrix3f &o)
{
  memcpy(&o.m00, m_readPointer, sizeof(float) * 9);
  m_readPointer += sizeof(float) * 9;
  return *this;
}

csAssetInputStream &csAssetInputStream::operator>>(csMatrix4f &o)
{
  memcpy(&o.m00, m_readPointer, sizeof(float) * 16);
  m_readPointer += sizeof(float) * 16;
  return *this;
}

csAssetInputStream &csAssetInputStream::operator>>(csString &o)
{
  csUInt16 length;
  *this >> length;

  char *buffer = new char[length + 1];
  memcpy(buffer, m_readPointer, length);
  buffer[length] = '\0';
  o = csString(buffer);

  m_readPointer += length;
  return *this;
}

csAssetInputStream &csAssetInputStream::Read(void *buffer, size_t size)
{
  memcpy(buffer, m_readPointer, size);
  m_readPointer += size;
  return *this;
}


const csUInt8 *csAssetInputStream::GetData() const
{
  return m_buffer;
}

const csUInt8 *csAssetInputStream::GetReadData() const
{
  return m_readPointer;
}

csSize csAssetInputStream::GetBufferSize() const
{
  return m_bufferSize;
}



