#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csmatrix3f.hh>
#include <cobalt/math/csmatrix4f.hh>
#include <cobalt/math/csvector2f.hh>
#include <cobalt/math/csvector3f.hh>
#include <cobalt/math/csvector4f.hh>


class CSE_API csAssetOutputStream
{
public:
  csAssetOutputStream(size_t initialCapacity = 64, size_t allocationSize = 32);
  virtual ~csAssetOutputStream();

  csAssetOutputStream & operator<< (const csInt8 &i);
  csAssetOutputStream & operator<< (const csUInt8 &i);
  csAssetOutputStream & operator<< (const csInt16 &i);
  csAssetOutputStream & operator<< (const csUInt16 &i);
  csAssetOutputStream & operator<< (const csInt32 &i);
  csAssetOutputStream & operator<< (const csUInt32 &i);
  csAssetOutputStream & operator<< (const size_t &i);
  csAssetOutputStream & operator<< (const float &i);
  csAssetOutputStream & operator<< (const csVector2f &i);
  csAssetOutputStream & operator<< (const csVector3f &i);
  csAssetOutputStream & operator<< (const csVector4f &i);
  csAssetOutputStream & operator<< (const csColor4f &i);
  csAssetOutputStream & operator<< (const csMatrix3f &i);
  csAssetOutputStream & operator<< (const csMatrix4f &i);
  csAssetOutputStream & operator<< (const std::string &i);
  csAssetOutputStream & Write(const void *buffer, size_t size);

  CS_FORCEINLINE const csUInt8 *GetBuffer() const
  {
    return m_buffer;
  }

  CS_FORCEINLINE size_t GetSize() const
  {
    return m_size;
  }

private:
  void AcquireCapacity(size_t capacity);
  csUInt8 *m_writePointer;
  csUInt8 *m_buffer;
  size_t m_size;
  size_t m_capacity;
  size_t m_allocationSize;
};
