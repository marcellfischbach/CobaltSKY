

#include <Valkyrie/Core/AssetStream.hh>


vkAssetInputStream::vkAssetInputStream(const vkUInt8 *buffer)
  : m_readPointer(buffer)
  , m_buffer(buffer)
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













vkAssetOutputStream::vkAssetOutputStream(vkUInt32 initialCapacity, vkUInt32 allocationSize)
  : m_buffer(0)
  , m_writePointer(0)
  , m_size(0)
  , m_capacity(initialCapacity)
  , m_allocationSize(allocationSize)
{
  m_buffer = new vkUInt8[m_capacity];
  m_writePointer = m_buffer;
}


vkAssetOutputStream::~vkAssetOutputStream()
{
  delete[] m_buffer;
  m_buffer = 0;
  m_writePointer = 0;
}

void vkAssetOutputStream::AcquireCapacity(vkUInt32 capacity)
{
  if ((m_size + capacity) > m_capacity)
  {
    vkUInt8 *newBuffer = new vkUInt8[m_size + m_capacity + m_allocationSize];
    memcpy(newBuffer, m_buffer, m_size);

    m_writePointer = newBuffer + (m_writePointer - m_buffer);
    delete[] m_buffer;
    m_buffer = newBuffer;
  }
}

vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkInt8 &i) 
{
  AcquireCapacity(sizeof(vkInt8));
  *reinterpret_cast<vkInt8*>(m_writePointer) = i;
  m_writePointer++;
  return *this;
}

vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkUInt8 &i)
{
  AcquireCapacity(sizeof(vkUInt8));
  *reinterpret_cast<vkUInt8*>(m_writePointer) = i;
  m_writePointer++;
  return *this;
}


vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkInt16 &i)
{
  AcquireCapacity(sizeof(vkInt16));
  *reinterpret_cast<vkInt16*>(m_writePointer) = i;
  m_writePointer+=sizeof(vkInt16);
  return *this;
}

vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkUInt16 &i)
{
  AcquireCapacity(sizeof(vkUInt16));
  *reinterpret_cast<vkUInt16*>(m_writePointer) = i;
  m_writePointer+=sizeof(vkUInt16);
  return *this;
}



vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkInt32 &i)
{
  AcquireCapacity(sizeof(vkInt32));
  *reinterpret_cast<vkInt32*>(m_writePointer) = i;
  m_writePointer += sizeof(vkInt32);
  return *this;
}

vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkUInt32 &i)
{
  AcquireCapacity(sizeof(vkUInt32));
  *reinterpret_cast<vkUInt32*>(m_writePointer) = i;
  m_writePointer += sizeof(vkUInt32);
  return *this;
}


vkAssetOutputStream & vkAssetOutputStream::operator<< (const float &i)
{
  AcquireCapacity(sizeof(float));
  *reinterpret_cast<float*>(m_writePointer) = i;
  m_writePointer += sizeof(float);
  return *this;
}


vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkVector2f &i)
{
  AcquireCapacity(sizeof(float)*2);
  *reinterpret_cast<float*>(m_writePointer) = i.x;
  m_writePointer += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.y;
  m_writePointer += sizeof(float);
  return *this;
}


vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkVector3f &i)
{
  AcquireCapacity(sizeof(float) * 3);
  *reinterpret_cast<float*>(m_writePointer) = i.x;
  m_writePointer += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.y;
  m_writePointer += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.z;
  m_writePointer += sizeof(float);
  return *this;
}

vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkVector4f &i)
{
  AcquireCapacity(sizeof(float) * 4);
  *reinterpret_cast<float*>(m_writePointer) = i.x;
  m_writePointer += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.y;
  m_writePointer += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.z;
  m_writePointer += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.w;
  m_writePointer += sizeof(float);
  return *this;
}

vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkColor4f &i)
{
  AcquireCapacity(sizeof(float) * 4);
  *reinterpret_cast<float*>(m_writePointer) = i.r;
  m_writePointer += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.g;
  m_writePointer += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.b;
  m_writePointer += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.a;
  m_writePointer += sizeof(float);
  return *this;
}

vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkMatrix3f &i)
{
  AcquireCapacity(sizeof(float) * 9);
  memcpy(reinterpret_cast<float*>(m_writePointer), &i.m00, sizeof(float) * 9);
  m_writePointer += sizeof(float) * 9;
  return *this;
}


vkAssetOutputStream &vkAssetOutputStream::operator<<(const vkMatrix4f &i)
{
  AcquireCapacity(sizeof(float) * 16);
  memcpy(reinterpret_cast<float*>(m_writePointer), &i.m00, sizeof(float) * 16);
  m_writePointer += sizeof(float) * 16;
  return *this;
}
