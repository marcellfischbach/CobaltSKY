#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Types.hh>
#include <Valkyrie/Math/Color.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Math/Vector.hh>

class VKE_API vkAssetInputStream
{
public:
  vkAssetInputStream(const vkUInt8 *buffer, vkSize bufferSize);
  virtual ~vkAssetInputStream();

  vkAssetInputStream & operator>> (vkInt8 &o);
  vkAssetInputStream & operator>> (vkUInt8 &o);
  vkAssetInputStream & operator>> (vkInt16 &o);
  vkAssetInputStream & operator>> (vkUInt16 &o);
  vkAssetInputStream & operator>> (vkInt32 &o);
  vkAssetInputStream & operator>> (vkUInt32 &o);
  vkAssetInputStream & operator>> (vkSize &o);
  vkAssetInputStream & operator>> (float &o);
  vkAssetInputStream & operator>> (vkVector2f &o);
  vkAssetInputStream & operator>> (vkVector3f &o);
  vkAssetInputStream & operator>> (vkVector4f &o);
  vkAssetInputStream & operator>> (vkColor4f &o);
  vkAssetInputStream & operator>> (vkMatrix3f &o);
  vkAssetInputStream & operator>> (vkMatrix4f &o);
  vkAssetInputStream & operator>> (vkString &o);
  vkAssetInputStream & Read(void *buffer, size_t size);

  const vkUInt8 *GetData() const;
  const vkUInt8 *GetReadData() const;
  vkSize GetBufferSize() const;

private:
  const vkUInt8 *m_readPointer;
  const vkUInt8 *m_buffer;
  vkSize m_bufferSize;
};


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