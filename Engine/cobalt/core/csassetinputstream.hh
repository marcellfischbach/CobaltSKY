#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csmatrix3f.hh>
#include <cobalt/math/csmatrix4f.hh>
#include <cobalt/math/csvector2f.hh>
#include <cobalt/math/csvector3f.hh>
#include <cobalt/math/csvector4f.hh>

class CSE_API csAssetInputStream
{
public:
  csAssetInputStream(const csUInt8 *buffer, csSize bufferSize);
  virtual ~csAssetInputStream();

  csAssetInputStream & operator>> (csInt8 &o);
  csAssetInputStream & operator>> (csUInt8 &o);
  csAssetInputStream & operator>> (csInt16 &o);
  csAssetInputStream & operator>> (csUInt16 &o);
  csAssetInputStream & operator>> (csInt32 &o);
  csAssetInputStream & operator>> (csUInt32 &o);
  csAssetInputStream & operator>> (csSize &o);
  csAssetInputStream & operator>> (float &o);
  csAssetInputStream & operator>> (csVector2f &o);
  csAssetInputStream & operator>> (csVector3f &o);
  csAssetInputStream & operator>> (csVector4f &o);
  csAssetInputStream & operator>> (csColor4f &o);
  csAssetInputStream & operator>> (csMatrix3f &o);
  csAssetInputStream & operator>> (csMatrix4f &o);
  csAssetInputStream & operator>> (std::string &o);
  csAssetInputStream & Read(void *buffer, size_t size);

  const csUInt8 *GetData() const;
  const csUInt8 *GetReadData() const;
  csSize GetBufferSize() const;

private:
  const csUInt8 *m_readPointer;
  const csUInt8 *m_buffer;
  csSize m_bufferSize;
};

