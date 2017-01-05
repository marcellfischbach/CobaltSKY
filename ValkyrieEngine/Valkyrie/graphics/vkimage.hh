#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkdefs.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/vkenums.hh>
#include <vector>
#include <Valkyrie/graphics/vkimage.refl.hh>


VK_CLASS();
class VKE_API vkImage : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;

public:
  vkImage();
  virtual ~vkImage();


  void SetData(vkSize size, const void *data);
  const void *GetData(vkUInt16 level = 0) const;

  void SetWidth(vkUInt16 width);
  void SetHeight(vkUInt16 height);
  void SetDepth(vkUInt16 depth);
  void SetPixelFormat(vkPixelFormat pixelFormat);

  vkUInt16 GetWidth(vkUInt16 level = 0) const;
  vkUInt16 GetHeight(vkUInt16 level = 0) const;
  vkUInt16 GetDepth(vkUInt16 level = 0) const;
  vkUInt8 GetNumberOfLevels() const;
  vkPixelFormat GetPixelFormat() const;
  vkUInt32 GetSize(vkUInt16 level = 0) const;

  bool GenerateMipMaps();

private:
  void Delete();
  vkUInt32 GetBytesPerPixel(vkPixelFormat format) const;

  vkUInt16 m_width;
  vkUInt16 m_height;
  vkUInt16 m_depth;
  vkPixelFormat m_pixelFormat;

  std::vector<vkUInt8*> m_data;
};




VK_FORCEINLINE void vkImage::SetPixelFormat(vkPixelFormat pixelFormat)
{
  m_pixelFormat = pixelFormat;
}


VK_FORCEINLINE vkPixelFormat vkImage::GetPixelFormat() const
{
  return m_pixelFormat;
}

VK_FORCEINLINE vkUInt8 vkImage::GetNumberOfLevels() const
{
  return (vkUInt8)m_data.size();
}