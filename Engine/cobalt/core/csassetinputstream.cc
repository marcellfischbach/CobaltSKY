

#include <cobalt/core/csassetinputstream.hh>


cs::AssetInputStream::AssetInputStream(const csUInt8 *buffer, csSize bufferSize)
  : m_readPointer(buffer)
  , m_buffer(buffer)
  , m_bufferSize(bufferSize)
{

}

cs::AssetInputStream::~AssetInputStream()
{
  m_buffer = 0;
  m_readPointer = 0;
}


cs::AssetInputStream &cs::AssetInputStream::operator>>(csInt8 &o)
{
  o = *reinterpret_cast<const csInt8*>(m_readPointer);
  m_readPointer++;
  return *this;
}


cs::AssetInputStream &cs::AssetInputStream::operator>>(csUInt8 &o)
{
  o = *reinterpret_cast<const csUInt8*>(m_readPointer);
  m_readPointer++;
  return *this;
}



cs::AssetInputStream &cs::AssetInputStream::operator>>(csInt16 &o)
{
  o = *reinterpret_cast<const csInt16*>(m_readPointer);
  m_readPointer += sizeof(csInt16);
  return *this;
}


cs::AssetInputStream &cs::AssetInputStream::operator>>(csUInt16 &o)
{
  o = *reinterpret_cast<const csUInt16*>(m_readPointer);
  m_readPointer += sizeof(csUInt16);
  return *this;
}


cs::AssetInputStream &cs::AssetInputStream::operator>>(csInt32 &o)
{
  o = *reinterpret_cast<const csInt32*>(m_readPointer);
  m_readPointer += sizeof(csInt32);
  return *this;
}


cs::AssetInputStream &cs::AssetInputStream::operator>>(csUInt32 &o)
{
  o = *reinterpret_cast<const csUInt32*>(m_readPointer);
  m_readPointer += sizeof(csUInt32);
  return *this;
}


cs::AssetInputStream &cs::AssetInputStream::operator>>(csSize &o)
{
  o = *reinterpret_cast<const csSize*>(m_readPointer);
  m_readPointer += sizeof(csSize);
  return *this;
}



cs::AssetInputStream &cs::AssetInputStream::operator>>(float &o)
{
  o = *reinterpret_cast<const float*>(m_readPointer);
  m_readPointer += sizeof(float);
  return *this;
}



cs::AssetInputStream &cs::AssetInputStream::operator>>(cs::Vector2f &o)
{
  *this >> o.x >> o.y;
  return *this;
}

cs::AssetInputStream &cs::AssetInputStream::operator>>(cs::Vector3f &o)
{
  *this >> o.x >> o.y >> o.z;
  return *this;
}

cs::AssetInputStream &cs::AssetInputStream::operator>>(cs::Vector4f &o)
{
  *this >> o.x >> o.y >> o.z >> o.w;
  return *this;
}


cs::AssetInputStream &cs::AssetInputStream::operator>>(cs::Color4f &o)
{
  *this >> o.r >> o.g >> o.b >> o.a;
  return *this;
}

cs::AssetInputStream &cs::AssetInputStream::operator>>(cs::Matrix3f &o)
{
  memcpy(&o.m00, m_readPointer, sizeof(float) * 9);
  m_readPointer += sizeof(float) * 9;
  return *this;
}

cs::AssetInputStream &cs::AssetInputStream::operator>>(cs::Matrix4f &o)
{
  memcpy(&o.m00, m_readPointer, sizeof(float) * 16);
  m_readPointer += sizeof(float) * 16;
  return *this;
}

cs::AssetInputStream &cs::AssetInputStream::operator>>(std::string &o)
{
  csUInt16 length;
  *this >> length;

  char *buffer = new char[static_cast<size_t>(length) + 1];
  memcpy(buffer, m_readPointer, length);
  buffer[length] = '\0';
  o = std::string(buffer);

  m_readPointer += length;
  return *this;
}

cs::AssetInputStream &cs::AssetInputStream::Read(void *buffer, size_t size)
{
  memcpy(buffer, m_readPointer, size);
  m_readPointer += size;
  return *this;
}


const csUInt8 *cs::AssetInputStream::GetData() const
{
  return m_buffer;
}

const csUInt8 *cs::AssetInputStream::GetReadData() const
{
  return m_readPointer;
}

csSize cs::AssetInputStream::GetBufferSize() const
{
  return m_bufferSize;
}



