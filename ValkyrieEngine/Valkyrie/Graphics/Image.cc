

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
  Delete();
}

void vkImage::SetData(vkSize size, const void *data)
{
  if (m_data.size() > 0)
  {
    Delete();
  }

  vkUInt8 *lData = new vkUInt8[size];
  memcpy(lData, data, size);
  m_data.push_back(lData);
}

void vkImage::Delete()
{
  for (vkUInt8 *data : m_data)
  {
    if (data)
    {
      delete[] data;
    }
  }

  m_data.clear();
}

namespace
{

vkUInt8 *generate_down_scale(const vkUInt8 *data, vkUInt16 width, vkUInt16 height, vkPixelFormat format)
{
  if (width == 1 && height == 1)
  {
    return 0;
  }

  vkUInt16 nWidth = width >> 1;
  if (nWidth == 0)
  {
    nWidth = 1;
  }

  vkUInt16 nHeight = height >> 1;
  if (nHeight == 0)
  {
    nHeight = 1;
  }


  int num = 0;
  switch (format)
  {
  case ePF_R8G8B8A8U:
    num = 4;
    break;
  case ePF_R8G8B8U:
    num = 3;
    break;
  case ePF_R8G8U:
    num = 2;
    break;
  case ePF_R8U:
    num = 1;
    break;
  }
  if (num == 0)
  {
    return 0;
  }

  bool debug = nWidth == 2 && nHeight == 2;

  vkUInt8 *res = new vkUInt8[nWidth * nHeight * num];
  vkSize lineScan = width * num;
  const vkUInt8 *srcPtr = data;
  vkUInt8 *dstPtr = res;

  for (vkUInt16 y = 0, sy = 0; y < nHeight; ++y, sy += 2)
  {
    bool bottom = (sy + 1) < height;
    for (vkUInt16 x = 0, sx = 0; x < nWidth; ++x, sx += 2)
    {
      bool right = (sx + 1) < width;
      for (int c = 0; c < num; ++c, ++srcPtr)
      {
        vkUInt16 col = *srcPtr;
        int rc = 1;

        if (right)
        {
          col += *(srcPtr + num);
          rc++;
        }
        if (bottom)
        {
          col += *(srcPtr + lineScan);
          rc++;

          if (right)
          {
            col += *(srcPtr + lineScan + num);
            rc++;
          }
        }

        float v = (float)col / (float)rc;
        *dstPtr++ = (vkUInt8)v;
      }
      srcPtr += num;
    }
    srcPtr += lineScan;
  }

  return res;
}

}

bool vkImage::GenerateMipMaps()
{
  if (m_data.size() != 1
      || m_depth != 1
      || !(m_pixelFormat == ePF_R8G8B8A8U || m_pixelFormat == ePF_R8G8B8U || m_pixelFormat == ePF_R8G8U || m_pixelFormat == ePF_R8U))
  {
    return false;
  }

  vkUInt16 width = m_width;
  vkUInt16 height = m_height;
  vkUInt16 idx = 0;
  while (true)
  {
    vkUInt8 *data = generate_down_scale(m_data[idx], width, height, m_pixelFormat);
    if (!data)
    {
      false;
    }

    m_data.push_back(data);

    if (width == 1 && height == 1)
    {
      break;
    }

    width >>= 1;
    if (width == 0)
    {
      width = 1;
    }

    height >>= 1;
    if (height == 0)
    {
      height = 1;
    }
  }



  return true;
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



vkUInt16 vkImage::GetWidth(vkUInt16 level) const
{
  vkUInt16 width = m_width >> level;
  if (width == 0)
  {
    width = 1;
  }
  return width;
}

vkUInt16 vkImage::GetHeight(vkUInt16 level) const
{
  vkUInt16 height = m_height >> level;
  if (height == 0)
  {
    height = 1;
  }
  return height;
}

vkUInt16 vkImage::GetDepth(vkUInt16 level) const
{
  vkUInt16 depth = m_depth >> level;
  if (depth == 0)
  {
    depth = 1;
  }
  return depth;
}


const void *vkImage::GetData(vkUInt16 level) const
{
  if (level >= m_data.size())
  {
    return 0;
  }
  return m_data[level];
}


