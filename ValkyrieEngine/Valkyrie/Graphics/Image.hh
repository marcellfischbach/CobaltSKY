#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Defs.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/Image.refl.hh>



VK_CLASS();
class VKE_API vkImage : public vkObject
{
  VK_CLASS_GEN;

public:
  vkImage();
  virtual ~vkImage();


  void SetData(vkSize size, const void *data);
  const void *GetData() const;

  void SetWidth(vkUInt16 width);
  void SetHeight(vkUInt16 height);
  void SetDepth(vkUInt16 depth);

  vkUInt16 GetWidth() const;
  vkUInt16 GetHeight() const;
  vkUInt16 GetDepth() const;

private:
  vkUInt16 m_width;
  vkUInt16 m_height;
  vkUInt16 m_depth;

  vkUInt8 *m_data;
};



VK_FORCEINLINE vkUInt16 vkImage::GetWidth() const 
{
  return m_width;
}

VK_FORCEINLINE vkUInt16 vkImage::GetHeight() const
{
  return m_height;
}

VK_FORCEINLINE vkUInt16 vkImage::GetDepth() const
{
  return m_depth;
}


VK_FORCEINLINE const void *vkImage::GetData() const
{
  return m_data;
}

