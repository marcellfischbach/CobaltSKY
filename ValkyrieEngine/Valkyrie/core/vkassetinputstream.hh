#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vktypes.hh>
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

