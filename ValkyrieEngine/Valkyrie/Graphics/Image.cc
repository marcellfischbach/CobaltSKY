

#include <Valkyrie/Graphics/Image.hh>


vkImage::vkImage()
  : vkObject()
  , m_width(0)
  , m_height(0)
  , m_depth(1)
  , m_data(0)
{

}

vkImage::~vkImage()
{
  if (m_data)
  {
    delete[] m_data;
  }
}

void vkImage::SetData(vkSize size, const void *data)
{
  if (m_data)
  {
    delete[] m_data;
  }
  m_data = new vkUInt8[size];
  memcpy(m_data, data, size);
}

void vkImage::SetWidth(vkUInt16 width)
{
  m_width = width > 0 ? width : 1;
}

void vkImage::SetHeight(vkUInt16 height)
{
  m_height = height > 0 ? height : 1;
}

void vkImage::SetDepth(vkUInt16 depth)
{
  m_depth = depth > 0 ? depth : 1;
}



