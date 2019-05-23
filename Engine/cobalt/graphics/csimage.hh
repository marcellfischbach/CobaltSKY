#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/csenums.hh>
#include <cobalt/cstypes.hh>
#include <vector>
#include <cobalt/graphics/csimage.refl.hh>

namespace cs
{

CS_CLASS();
class CSE_API Image : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  Image();
  virtual ~Image();


  void SetData(csSize size, const void* data);
  const void* GetData(csUInt16 level = 0) const;

  void SetWidth(csUInt16 width);
  void SetHeight(csUInt16 height);
  void SetDepth(csUInt16 depth);
  void SetPixelFormat(cs::ePixelFormat pixelFormat);

  csUInt16 GetWidth(csUInt16 level = 0) const;
  csUInt16 GetHeight(csUInt16 level = 0) const;
  csUInt16 GetDepth(csUInt16 level = 0) const;
  csUInt8 GetNumberOfLevels() const;
  cs::ePixelFormat GetPixelFormat() const;
  csUInt32 GetSize(csUInt16 level = 0) const;

  bool GenerateMipMaps(bool normal);

private:
  void Delete();
  csUInt32 GetBytesPerPixel(cs::ePixelFormat format) const;

  csUInt16 m_width;
  csUInt16 m_height;
  csUInt16 m_depth;
  cs::ePixelFormat m_pixelFormat;

  std::vector<csUInt8*> m_data;
};

CS_CLASS()
class CSE_API ImageWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::Image, ImageWrapper, cs::ResourceWrapper);
};

}

CS_FORCEINLINE void cs::Image::SetPixelFormat(cs::ePixelFormat pixelFormat)
{
  m_pixelFormat = pixelFormat;
}


CS_FORCEINLINE cs::ePixelFormat cs::Image::GetPixelFormat() const
{
  return m_pixelFormat;
}

CS_FORCEINLINE csUInt8 cs::Image::GetNumberOfLevels() const
{
  return (csUInt8)m_data.size();
}
