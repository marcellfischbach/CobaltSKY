

#include <Valkyrie/Core/vkassetinputstream.hh>


vkAssetInputStream::vkAssetInputStream(const vkUInt8 *buffer, vkSize bufferSize)
  : m_readPointer(buffer)
  , m_buffer(buffer)
  , m_bufferSize(bufferSize)
{

}

vkAssetInputStream::~vkAssetInputStream()
{
  m_buffer = 0;
  m_readPointer = 0;
}


vkAssetInputStream &vkAssetInputStream::operator>>(vkInt8 &o)
{
  o = *reinterpret_cast<const vkInt8*>(m_readPointer);
  m_readPointer++;
  return *this;
}


vkAssetInputStream &vkAssetInputStream::operator>>(vkUInt8 &o)
{
  o = *reinterpret_cast<const vkUInt8*>(m_readPointer);
  m_readPointer++;
  return *this;
}



vkAssetInputStream &vkAssetInputStream::operator>>(vkInt16 &o)
{
  o = *reinterpret_cast<const vkInt16*>(m_readPointer);
  m_readPointer += sizeof(vkInt16);
  return *this;
}


vkAssetInputStream &vkAssetInputStream::operator>>(vkUInt16 &o)
{
  o = *reinterpret_cast<const vkUInt16*>(m_readPointer);
  m_readPointer += sizeof(vkUInt16);
  return *this;
}


vkAssetInputStream &vkAssetInputStream::operator>>(vkInt32 &o)
{
  o = *reinterpret_cast<const vkInt32*>(m_readPointer);
  m_readPointer += sizeof(vkInt32);
  return *this;
}


vkAssetInputStream &vkAssetInputStream::operator>>(vkUInt32 &o)
{
  o = *reinterpret_cast<const vkUInt32*>(m_readPointer);
  m_readPointer += sizeof(vkUInt32);
  return *this;
}


vkAssetInputStream &vkAssetInputStream::operator>>(vkSize &o)
{
  o = *reinterpret_cast<const vkSize*>(m_readPointer);
  m_readPointer += sizeof(vkSize);
  return *this;
}



vkAssetInputStream &vkAssetInputStream::operator>>(float &o)
{
  o = *reinterpret_cast<const float*>(m_readPointer);
  m_readPointer += sizeof(float);
  return *this;
}



vkAssetInputStream &vkAssetInputStream::operator>>(vkVector2f &o)
{
  *this >> o.x >> o.y;
  return *this;
}

vkAssetInputStream &vkAssetInputStream::operator>>(vkVector3f &o)
{
  *this >> o.x >> o.y >> o.z;
  return *this;
}

vkAssetInputStream &vkAssetInputStream::operator>>(vkVector4f &o)
{
  *this >> o.x >> o.y >> o.z >> o.w;
  return *this;
}


vkAssetInputStream &vkAssetInputStream::operator>>(vkColor4f &o)
{
  *this >> o.r >> o.g >> o.b >> o.a;
  return *this;
}

vkAssetInputStream &vkAssetInputStream::operator>>(vkMatrix3f &o)
{
  memcpy(&o.m00, m_readPointer, sizeof(float) * 9);
  m_readPointer += sizeof(float) * 9;
  return *this;
}

vkAssetInputStream &vkAssetInputStream::operator>>(vkMatrix4f &o)
{
  memcpy(&o.m00, m_readPointer, sizeof(float) * 16);
  m_readPointer += sizeof(float) * 16;
  return *this;
}

vkAssetInputStream &vkAssetInputStream::operator>>(vkString &o)
{
  vkUInt16 length;
  *this >> length;

  char *buffer = new char[length + 1];
  memcpy(buffer, m_readPointer, length);
  buffer[length] = '\0';
  o = vkString(buffer);

  m_readPointer += length;
  return *this;
}

vkAssetInputStream &vkAssetInputStream::Read(void *buffer, size_t size)
{
  memcpy(buffer, m_readPointer, size);
  m_readPointer += size;
  return *this;
}


const vkUInt8 *vkAssetInputStream::GetData() const
{
  return m_buffer;
}

const vkUInt8 *vkAssetInputStream::GetReadData() const
{
  return m_readPointer;
}

vkSize vkAssetInputStream::GetBufferSize() const
{
  return m_bufferSize;
}



