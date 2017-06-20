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
  csAssetOutputStream(csUInt32 initialCapacity = 64, csUInt32 allocationSize = 32);
  virtual ~csAssetOutputStream();

  csAssetOutputStream & operator<< (const csInt8 &i);
  csAssetOutputStream & operator<< (const csUInt8 &i);
  csAssetOutputStream & operator<< (const csInt16 &i);
  csAssetOutputStream & operator<< (const csUInt16 &i);
  csAssetOutputStream & operator<< (const csInt32 &i);
  csAssetOutputStream & operator<< (const csUInt32 &i);
  csAssetOutputStream & operator<< (const csSize &i);
  csAssetOutputStream & operator<< (const float &i);
  csAssetOutputStream & operator<< (const csVector2f &i);
  csAssetOutputStream & operator<< (const csVector3f &i);
  csAssetOutputStream & operator<< (const csVector4f &i);
  csAssetOutputStream & operator<< (const csColor4f &i);
  csAssetOutputStream & operator<< (const csMatrix3f &i);
  csAssetOutputStream & operator<< (const csMatrix4f &i);
  csAssetOutputStream & operator<< (const csString &i);
  csAssetOutputStream & Write(const void *buffer, size_t size);

  CS_FORCEINLINE const csUInt8 *GetBuffer() const
  {
    return m_buffer;
  }

  CS_FORCEINLINE csUInt32 GetSize() const
  {
    return m_size;
  }

private:
  void AcquireCapacity(csUInt32 capacity);
  csUInt8 *m_writePointer;
  csUInt8 *m_buffer;
  csUInt32 m_size;
  csUInt32 m_capacity;
  csUInt32 m_allocationSize;
};
