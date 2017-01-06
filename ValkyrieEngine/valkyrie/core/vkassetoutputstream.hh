#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vktypes.hh>
#include <valkyrie/math/vkcolor4f.hh>
#include <valkyrie/math/vkmatrix3f.hh>
#include <valkyrie/math/vkmatrix4f.hh>
#include <valkyrie/math/vkvector2f.hh>
#include <valkyrie/math/vkvector3f.hh>
#include <valkyrie/math/vkvector4f.hh>


class VKE_API vkAssetOutputStream
{
public:
  vkAssetOutputStream(vkUInt32 initialCapacity = 64, vkUInt32 allocationSize = 32);
  virtual ~vkAssetOutputStream();

  vkAssetOutputStream & operator<< (const vkInt8 &i);
  vkAssetOutputStream & operator<< (const vkUInt8 &i);
  vkAssetOutputStream & operator<< (const vkInt16 &i);
  vkAssetOutputStream & operator<< (const vkUInt16 &i);
  vkAssetOutputStream & operator<< (const vkInt32 &i);
  vkAssetOutputStream & operator<< (const vkUInt32 &i);
  vkAssetOutputStream & operator<< (const vkSize &i);
  vkAssetOutputStream & operator<< (const float &i);
  vkAssetOutputStream & operator<< (const vkVector2f &i);
  vkAssetOutputStream & operator<< (const vkVector3f &i);
  vkAssetOutputStream & operator<< (const vkVector4f &i);
  vkAssetOutputStream & operator<< (const vkColor4f &i);
  vkAssetOutputStream & operator<< (const vkMatrix3f &i);
  vkAssetOutputStream & operator<< (const vkMatrix4f &i);
  vkAssetOutputStream & operator<< (const vkString &i);
  vkAssetOutputStream & Write(const void *buffer, size_t size);

  VK_FORCEINLINE const vkUInt8 *GetBuffer() const
  {
    return m_buffer;
  }

  VK_FORCEINLINE vkUInt32 GetSize() const
  {
    return m_size;
  }

private:
  void AcquireCapacity(vkUInt32 capacity);
  vkUInt8 *m_writePointer;
  vkUInt8 *m_buffer;
  vkUInt32 m_size;
  vkUInt32 m_capacity;
  vkUInt32 m_allocationSize;
};
