

#include <cobalt/core/csassetoutputstream.hh>







csAssetOutputStream::csAssetOutputStream(csUInt32 initialCapacity, csUInt32 allocationSize)
  : m_buffer(0)
  , m_writePointer(0)
  , m_size(0)
  , m_capacity(initialCapacity)
  , m_allocationSize(allocationSize)
{
  m_buffer = new csUInt8[m_capacity];
  m_writePointer = m_buffer;
}


csAssetOutputStream::~csAssetOutputStream()
{
  delete[] m_buffer;
  m_buffer = 0;
  m_writePointer = 0;
}

void csAssetOutputStream::AcquireCapacity(csUInt32 capacity)
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

csAssetOutputStream &csAssetOutputStream::operator<<(const csInt8 &i) 
{
  AcquireCapacity(sizeof(csInt8));
  *reinterpret_cast<csInt8*>(m_writePointer) = i;
  m_writePointer++;
  m_size += 1;
  return *this;
}

csAssetOutputStream &csAssetOutputStream::operator<<(const csUInt8 &i)
{
  AcquireCapacity(sizeof(csUInt8));
  *reinterpret_cast<csUInt8*>(m_writePointer) = i;
  m_writePointer++;
  m_size += 1;
  return *this;
}


csAssetOutputStream &csAssetOutputStream::operator<<(const csInt16 &i)
{
  AcquireCapacity(sizeof(csInt16));
  *reinterpret_cast<csInt16*>(m_writePointer) = i;
  m_writePointer+=sizeof(csInt16);
  m_size += sizeof(csInt16);
  return *this;
}

csAssetOutputStream &csAssetOutputStream::operator<<(const csUInt16 &i)
{
  AcquireCapacity(sizeof(csUInt16));
  *reinterpret_cast<csUInt16*>(m_writePointer) = i;
  m_writePointer+=sizeof(csUInt16);
  m_size += sizeof(csUInt16);
  return *this;
}



csAssetOutputStream &csAssetOutputStream::operator<<(const csInt32 &i)
{
  AcquireCapacity(sizeof(csInt32));
  *reinterpret_cast<csInt32*>(m_writePointer) = i;
  m_writePointer += sizeof(csInt32);
  m_size += sizeof(csInt32);
  return *this;
}

csAssetOutputStream &csAssetOutputStream::operator<<(const csUInt32 &i)
{
  AcquireCapacity(sizeof(csUInt32));
  *reinterpret_cast<csUInt32*>(m_writePointer) = i;
  m_writePointer += sizeof(csUInt32);
  m_size += sizeof(csUInt32);
  return *this;
}


csAssetOutputStream &csAssetOutputStream::operator<<(const csSize &i)
{
  AcquireCapacity(sizeof(csSize));
  *reinterpret_cast<csSize*>(m_writePointer) = i;
  m_writePointer += sizeof(csSize);
  m_size += sizeof(csSize);
  return *this;
}


csAssetOutputStream & csAssetOutputStream::operator<< (const float &i)
{
  AcquireCapacity(sizeof(float));
  *reinterpret_cast<float*>(m_writePointer) = i;
  m_writePointer += sizeof(float);
  m_size += sizeof(float);
  return *this;
}


csAssetOutputStream &csAssetOutputStream::operator<<(const csVector2f &i)
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


csAssetOutputStream &csAssetOutputStream::operator<<(const csVector3f &i)
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

csAssetOutputStream &csAssetOutputStream::operator<<(const csVector4f &i)
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

csAssetOutputStream &csAssetOutputStream::operator<<(const csColor4f &i)
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

csAssetOutputStream &csAssetOutputStream::operator<<(const csMatrix3f &i)
{
  AcquireCapacity(sizeof(float) * 9);
  memcpy(reinterpret_cast<float*>(m_writePointer), &i.m00, sizeof(float) * 9);
  m_writePointer += sizeof(float) * 9;
  m_size += sizeof(float) * 9;

  return *this;
}


csAssetOutputStream &csAssetOutputStream::operator<<(const csMatrix4f &i)
{
  AcquireCapacity(sizeof(float) * 16);
  memcpy(reinterpret_cast<float*>(m_writePointer), &i.m00, sizeof(float) * 16);
  m_writePointer += sizeof(float) * 16;
  m_size += sizeof(float) * 16;
  return *this;
}

csAssetOutputStream &csAssetOutputStream::operator<<(const std::string &i)
{
  AcquireCapacity(sizeof(csUInt16) + i.length());
  *this << (csUInt16)i.length();

  memcpy(m_writePointer, i.c_str(), sizeof(char) * i.length());
  m_writePointer += sizeof(char) * i.length();
  m_size += sizeof(char) * i.length();
  return *this;
}

csAssetOutputStream &csAssetOutputStream::Write(const void *buffer, size_t size)
{
  AcquireCapacity(size);

  memcpy(m_writePointer, buffer, size);
  m_writePointer += size;
  m_size += size;
  return *this;
}
