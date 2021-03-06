

#include <cobalt/core/csassetoutputstream.hh>







cs::AssetOutputStream::AssetOutputStream(size_t initialCapacity, size_t allocationSize)
  : m_buffer(0)
  , m_writePointer(0)
  , m_size(0)
  , m_capacity(initialCapacity)
  , m_allocationSize(allocationSize)
{
  m_buffer = new csUInt8[m_capacity];
  m_writePointer = m_buffer;
}


cs::AssetOutputStream::~AssetOutputStream()
{
  delete[] m_buffer;
  m_buffer = 0;
  m_writePointer = 0;
}

void cs::AssetOutputStream::AcquireCapacity(size_t capacity)
{
  if ((m_size + capacity) > m_capacity)
  {
    csUInt8 *newBuffer = new csUInt8[m_size + m_capacity + m_allocationSize];
    memcpy(newBuffer, m_buffer, m_size);

    m_writePointer = newBuffer + (m_writePointer - m_buffer);
    delete[] m_buffer;
    m_buffer = newBuffer;
  }
}

cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const csInt8 &i) 
{
  AcquireCapacity(sizeof(csInt8));
  *reinterpret_cast<csInt8*>(m_writePointer) = i;
  m_writePointer++;
  m_size += 1;
  return *this;
}

cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const csUInt8 &i)
{
  AcquireCapacity(sizeof(csUInt8));
  *reinterpret_cast<csUInt8*>(m_writePointer) = i;
  m_writePointer++;
  m_size += 1;
  return *this;
}


cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const csInt16 &i)
{
  AcquireCapacity(sizeof(csInt16));
  *reinterpret_cast<csInt16*>(m_writePointer) = i;
  m_writePointer+=sizeof(csInt16);
  m_size += sizeof(csInt16);
  return *this;
}

cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const csUInt16 &i)
{
  AcquireCapacity(sizeof(csUInt16));
  *reinterpret_cast<csUInt16*>(m_writePointer) = i;
  m_writePointer+=sizeof(csUInt16);
  m_size += sizeof(csUInt16);
  return *this;
}



cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const csInt32 &i)
{
  AcquireCapacity(sizeof(csInt32));
  *reinterpret_cast<csInt32*>(m_writePointer) = i;
  m_writePointer += sizeof(csInt32);
  m_size += sizeof(csInt32);
  return *this;
}

cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const csUInt32 &i)
{
  AcquireCapacity(sizeof(csUInt32));
  *reinterpret_cast<csUInt32*>(m_writePointer) = i;
  m_writePointer += sizeof(csUInt32);
  m_size += sizeof(csUInt32);
  return *this;
}


cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const size_t &i)
{
  AcquireCapacity(sizeof(size_t));
  *reinterpret_cast<size_t*>(m_writePointer) = i;
  m_writePointer += sizeof(size_t);
  m_size += sizeof(size_t);
  return *this;
}


cs::AssetOutputStream & cs::AssetOutputStream::operator<< (const float &i)
{
  AcquireCapacity(sizeof(float));
  *reinterpret_cast<float*>(m_writePointer) = i;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  return *this;
}


cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const cs::Vector2f &i)
{
  AcquireCapacity(sizeof(float)*2);
  *reinterpret_cast<float*>(m_writePointer) = i.x;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.y;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  return *this;
}


cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const cs::Vector3f &i)
{
  AcquireCapacity(sizeof(float) * 3);
  *reinterpret_cast<float*>(m_writePointer) = i.x;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.y;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.z;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  return *this;
}

cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const cs::Vector4f &i)
{
  AcquireCapacity(sizeof(float) * 4);
  *reinterpret_cast<float*>(m_writePointer) = i.x;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.y;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.z;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.w;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  return *this;
}

cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const cs::Color4f &i)
{
  AcquireCapacity(sizeof(float) * 4);
  *reinterpret_cast<float*>(m_writePointer) = i.r;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.g;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.b;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  *reinterpret_cast<float*>(m_writePointer) = i.a;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  return *this;
}

cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const cs::Matrix3f &i)
{
  AcquireCapacity(sizeof(float) * 9);
  memcpy(reinterpret_cast<float*>(m_writePointer), &i.m00, sizeof(float) * 9);
  m_writePointer += sizeof(float) * 9;
  m_size += sizeof(float) * 9;

  return *this;
}


cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const cs::Matrix4f &i)
{
  AcquireCapacity(sizeof(float) * 16);
  memcpy(reinterpret_cast<float*>(m_writePointer), &i.m00, sizeof(float) * 16);
  m_writePointer += sizeof(float) * 16;
  m_size += sizeof(float) * 16;
  return *this;
}

cs::AssetOutputStream &cs::AssetOutputStream::operator<<(const std::string &i)
{
  AcquireCapacity(sizeof(csUInt16) + i.length());
  *this << (csUInt16)i.length();

  memcpy(m_writePointer, i.c_str(), sizeof(char) * i.length());
  m_writePointer += sizeof(char) * i.length();
  m_size += sizeof(char) * i.length();
  return *this;
}

cs::AssetOutputStream &cs::AssetOutputStream::Write(const void *buffer, size_t size)
{
  AcquireCapacity(size);

  memcpy(m_writePointer, buffer, size);
  m_writePointer += size;
  m_size += size;
  return *this;
}
