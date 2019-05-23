#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csmatrix3f.hh>
#include <cobalt/math/csmatrix4f.hh>
#include <cobalt/math/csvector2f.hh>
#include <cobalt/math/csvector3f.hh>
#include <cobalt/math/csvector4f.hh>


namespace cs
{

class CSE_API AssetOutputStream
{
public:
  AssetOutputStream(size_t initialCapacity = 64, size_t allocationSize = 32);
  virtual ~AssetOutputStream();

  cs::AssetOutputStream& operator<< (const csInt8& i);
  cs::AssetOutputStream& operator<< (const csUInt8& i);
  cs::AssetOutputStream& operator<< (const csInt16& i);
  cs::AssetOutputStream& operator<< (const csUInt16& i);
  cs::AssetOutputStream& operator<< (const csInt32& i);
  cs::AssetOutputStream& operator<< (const csUInt32& i);
  cs::AssetOutputStream& operator<< (const size_t& i);
  cs::AssetOutputStream& operator<< (const float& i);
  cs::AssetOutputStream& operator<< (const cs::Vector2f& i);
  cs::AssetOutputStream& operator<< (const cs::Vector3f& i);
  cs::AssetOutputStream& operator<< (const cs::Vector4f& i);
  cs::AssetOutputStream& operator<< (const cs::Color4f& i);
  cs::AssetOutputStream& operator<< (const cs::Matrix3f& i);
  cs::AssetOutputStream& operator<< (const cs::Matrix4f& i);
  cs::AssetOutputStream& operator<< (const std::string& i);
  cs::AssetOutputStream& Write(const void* buffer, size_t size);

  CS_FORCEINLINE const csUInt8* GetBuffer() const
  {
    return m_buffer;
  }

  CS_FORCEINLINE size_t GetSize() const
  {
    return m_size;
  }

private:
  void AcquireCapacity(size_t capacity);
  csUInt8* m_writePointer;
  csUInt8* m_buffer;
  size_t m_size;
  size_t m_capacity;
  size_t m_allocationSize;
};

}

