

#include <cobalt/graphics/csimage.hh>


csImage::csImage()
  : csObject()
  , m_width(0)
  , m_height(0)
  , m_depth(1)
  , m_data(0)
{

}

csImage::~csImage()
{
  Delete();
}

void csImage::SetData(csSize size, const void *data)
{
  if (m_data.size() > 0)
  {
    Delete();
  }

  csUInt8 *lData = new csUInt8[size];
  memcpy(lData, data, size);
  m_data.push_back(lData);
}

void csImage::Delete()
{
  for (csUInt8 *data : m_data)
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

  csUInt8 *generate_down_scale(const csUInt8 *data, csUInt16 srcWidth, csUInt16 srcHeight, csUInt16 width, csUInt16 height, csPixelFormat format)
  {
    if (width == 0 || height == 0)
    {
      return 0;
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

    unsigned sampleWidth = srcWidth / width;
    unsigned sampleHeight = srcHeight / height;

    csSize lineScan = width * num;
    if (lineScan % 4)
    {
      lineScan += 4 - (lineScan % 4);
    }
    csUInt8 *res = new csUInt8[height * lineScan];
    const csUInt8 *srcPtr = data;
    csUInt8 *dstPtr = res;
    for (csUInt16 dy = 0; dy < height; ++dy)
    {
      dstPtr = res + dy * lineScan;
      unsigned srcY0 = dy * sampleHeight;
      unsigned srcY1 = srcY0 + sampleHeight;
      for (csUInt16 dx = 0; dx < width; ++dx)
      {
        unsigned srcX0 = dx * sampleWidth;
        unsigned srcX1 = srcX0 + sampleWidth;
        for (csUInt8 c = 0; c < num; ++c)
        {
          unsigned value = 0;
          unsigned count = 0;
          const csUInt8* srcShift = srcPtr + c;
          for (csUInt16 sy = srcY0; sy < srcY1 && sy < srcHeight; ++sy)
          {
            const csUInt8 *srcPtrY = srcShift + sy * srcWidth * num;
            for (csUInt16 sx = srcX0; sx < srcX1 && sy < srcWidth; ++sx)
            {
              const csUInt8 *srcPtrX = srcPtrY + sx * num;
              value += *srcPtrX;
              count++;
            }
          }
          if (count)
          {
            value /= count;
          }
          *dstPtr++ = (csUInt8)value;
        }
      }
    }

    return res;
  }

  csUInt8 *generate_down_scale(const csUInt8 *data, csUInt16 width, csUInt16 height, csPixelFormat format)
  {
    if (width == 1 && height == 1)
    {
      return 0;
    }

    csUInt16 nWidth = width >> 1;
    if (nWidth == 0)
    {
      nWidth = 1;
    }

    csUInt16 nHeight = height >> 1;
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

    csUInt8 *res = new csUInt8[nWidth * nHeight * num];
    csSize lineScan = width * num;
    const csUInt8 *srcPtr = data;
    csUInt8 *dstPtr = res;

    for (csUInt16 dy = 0, sy = 0; dy < nHeight; ++dy, sy += 2)
    {
      bool bottom = (sy + 1) < height;
      for (csUInt16 dx = 0, sx = 0; dx < nWidth; ++dx, sx += 2)
      {
        bool right = (sx + 1) < width;
        for (int c = 0; c < num; ++c, ++srcPtr)
        {
          csUInt16 col = (csUInt8)*srcPtr;
          int rc = 1;
          if (right)
          {
            col += (csUInt8)*(srcPtr + num);
            rc++;
          }
          if (bottom)
          {
            col += (csUInt8)*(srcPtr + lineScan);
            rc++;

            if (right)
            {
              col += (csUInt8)*(srcPtr + lineScan + num);
              rc++;
            }
          }
          float v = (float)col / (float)rc;
          *dstPtr++ = (csUInt8)v;
        }
        srcPtr += num;
      }
      srcPtr += lineScan;
    }

    return res;
  }

}

bool csImage::GenerateMipMaps()
{
  if (m_data.size() != 1
    || m_depth != 1
    || !(m_pixelFormat == ePF_R8G8B8A8U || m_pixelFormat == ePF_R8G8B8U || m_pixelFormat == ePF_R8G8U || m_pixelFormat == ePF_R8U))
  {
    return false;
  }

  csUInt16 width = m_width;
  csUInt16 height = m_height;
  csUInt16 idx = 0;
#define DEEP_MIP_MAP
  while (true)
  {
#ifdef DEEP_MIP_MAP
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
    csUInt8 *data = generate_down_scale(m_data[0], m_width, m_height, width, height, m_pixelFormat);
    if (!data)
    {
      return false;
    }
    m_data.push_back(data);
    if (width == 1 && height == 1)
    {
      break;
    }
#else
    csUInt8 *data = generate_down_scale(m_data[idx++], width, height, m_pixelFormat);
    if (!data)
    {
      return false;
    }

    m_data.push_back(data);

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


    if (width == 1 && height == 1)
    {
      break;
    }
#endif

  }



  return true;
}



void csImage::SetWidth(csUInt16 width)
{
  m_width = width > 0 ? width : 1;
}

void csImage::SetHeight(csUInt16 height)
{
  m_height = height > 0 ? height : 1;
}

void csImage::SetDepth(csUInt16 depth)
{
  m_depth = depth > 0 ? depth : 1;
}



csUInt16 csImage::GetWidth(csUInt16 level) const
{
  csUInt16 width = m_width >> level;
  if (width == 0)
  {
    width = 1;
  }
  return width;
}

csUInt16 csImage::GetHeight(csUInt16 level) const
{
  csUInt16 height = m_height >> level;
  if (height == 0)
  {
    height = 1;
  }
  return height;
}

csUInt16 csImage::GetDepth(csUInt16 level) const
{
  csUInt16 depth = m_depth >> level;
  if (depth == 0)
  {
    depth = 1;
  }
  return depth;
}


const void *csImage::GetData(csUInt16 level) const
{
  if (level >= m_data.size())
  {
    return 0;
  }
  return m_data[level];
}


csUInt32 csImage::GetSize(csUInt16 level) const
{
  csUInt32 width = m_width >> level;
  if (width == 0)
  {
    width = 1;
  }
  csUInt32 height = m_height >> level;
  if (height == 0)
  {
    height = 1;
  }

  csUInt32 bpp = GetBytesPerPixel(m_pixelFormat);

  return width * height * bpp;
}

csUInt32 csImage::GetBytesPerPixel(csPixelFormat pixelFormat) const
{
  switch (pixelFormat)
  {
  case ePF_R8G8B8A8U:
    return 4;
  case ePF_R8G8B8U:
    return 3;
  case ePF_R8G8U:
    return 2;
  case ePF_R8U:
    return 1;
  }
  return 0;
}