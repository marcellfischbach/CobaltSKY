#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/csenums.hh>
#include <cobalt/cstypes.hh>
#include <vector>
#include <cobalt/graphics/csimage.refl.hh>


CS_CLASS();
class CSE_API csImage : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  csImage();
  virtual ~csImage();


  void SetData(csSize size, const void *data);
  const void *GetData(csUInt16 level = 0) const;

  void SetWidth(csUInt16 width);
  void SetHeight(csUInt16 height);
  void SetDepth(csUInt16 depth);
  void SetPixelFormat(csPixelFormat pixelFormat);

  csUInt16 GetWidth(csUInt16 level = 0) const;
  csUInt16 GetHeight(csUInt16 level = 0) const;
  csUInt16 GetDepth(csUInt16 level = 0) const;
  csUInt8 GetNumberOfLevels() const;
  csPixelFormat GetPixelFormat() const;
  csUInt32 GetSize(csUInt16 level = 0) const;

  bool GenerateMipMaps(bool normal);
  
private:
  void Delete();
  csUInt32 GetBytesPerPixel(csPixelFormat format) const;

  csUInt16 m_width;
  csUInt16 m_height;
  csUInt16 m_depth;
  csPixelFormat m_pixelFormat;

  std::vector<csUInt8*> m_data;
};

CS_CLASS()
class CSE_API csImageWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csImage, csImageWrapper, csResourceWrapper);
};


CS_FORCEINLINE void csImage::SetPixelFormat(csPixelFormat pixelFormat)
{
  m_pixelFormat = pixelFormat;
}


CS_FORCEINLINE csPixelFormat csImage::GetPixelFormat() const
{
  return m_pixelFormat;
}

CS_FORCEINLINE csUInt8 csImage::GetNumberOfLevels() const
{
  return (csUInt8)m_data.size();
}
