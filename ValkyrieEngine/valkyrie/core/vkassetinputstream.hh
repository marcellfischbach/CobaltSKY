#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vktypes.hh>
#include <valkyrie/math/vkcolor4f.hh>
#include <valkyrie/math/vkmatrix3f.hh>
#include <valkyrie/math/vkmatrix4f.hh>
#include <valkyrie/math/vkvector2f.hh>
#include <valkyrie/math/vkvector3f.hh>
#include <valkyrie/math/vkvector4f.hh>

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

