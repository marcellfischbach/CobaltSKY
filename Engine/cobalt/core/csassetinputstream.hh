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

class CSE_API AssetInputStream
{
public:
  AssetInputStream(const csUInt8* buffer, csSize bufferSize);
  virtual ~AssetInputStream();

  cs::AssetInputStream& operator>> (csInt8& o);
  cs::AssetInputStream& operator>> (csUInt8& o);
  cs::AssetInputStream& operator>> (csInt16& o);
  cs::AssetInputStream& operator>> (csUInt16& o);
  cs::AssetInputStream& operator>> (csInt32& o);
  cs::AssetInputStream& operator>> (csUInt32& o);
  cs::AssetInputStream& operator>> (csSize& o);
  cs::AssetInputStream& operator>> (float& o);
  cs::AssetInputStream& operator>> (cs::Vector2f& o);
  cs::AssetInputStream& operator>> (cs::Vector3f& o);
  cs::AssetInputStream& operator>> (cs::Vector4f& o);
  cs::AssetInputStream& operator>> (cs::Color4f& o);
  cs::AssetInputStream& operator>> (cs::Matrix3f& o);
  cs::AssetInputStream& operator>> (cs::Matrix4f& o);
  cs::AssetInputStream& operator>> (std::string& o);
  cs::AssetInputStream& Read(void* buffer, size_t size);

  const csUInt8* GetData() const;
  const csUInt8* GetReadData() const;
  csSize GetBufferSize() const;

private:
  const csUInt8* m_readPointer;
  const csUInt8* m_buffer;
  csSize m_bufferSize;
};


}

