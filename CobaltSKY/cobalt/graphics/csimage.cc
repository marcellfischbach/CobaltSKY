

#include <cobalt/graphics/csimage.hh>
#include <cobalt/math/csvector3f.hh>


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

  csInt16 convert(csUInt8 value, bool normal)
  {
    if (!normal)
    {
      return value;
    }
    csInt16 v = value;
    v -= 128;
    v *= 2;

    return v;
  }

  csInt16 convertBack(csInt16 value, bool normal)
  {
    if (!normal)
    {
      return value;
    }

    return value / 2 + 128;
  }

  csUInt8 *generate_down_scale(const csUInt8 *data, csUInt16 width, csUInt16 height, csPixelFormat format, bool normal)
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

    printf("generate_down_scale: %d %d => %d %d => %d   LS:  src: %d  dst: %d\n", width, height, nWidth, nHeight, num, width * num, nWidth * num);

    bool debug = nWidth == 2 && nHeight == 2;

    csSize srcLineScan = width * num;
    if (srcLineScan % 4)
    {
      printf("Correct srcLineScan %d % 4 => %d", srcLineScan, srcLineScan % 4);
      //srcLineScan += 4 - (srcLineScan % 4);
      printf(" => %d\n", srcLineScan);
    }

    csSize dstLineScan = nWidth * num;
    if (dstLineScan % 4)
    {
      printf("Correct dstLineScan %d % 4 => %d", dstLineScan, dstLineScan % 4);
      //dstLineScan += 4 - (dstLineScan % 4);
      printf(" => %d\n", dstLineScan);
    }

    csUInt8 *res = new csUInt8[nHeight * dstLineScan];

    for (csUInt16 dy = 0, sy = 0; dy < nHeight; ++dy, sy += 2)
    {
      bool bottom = (sy + 1) < height;
      csUInt16 dx = 0;

      csUInt8 *dstPtrY = res + dy * dstLineScan;
      const csUInt8 *srcPtrY = data + sy * srcLineScan;
      for (csUInt16 sx = 0; dx < nWidth; ++dx, sx += 2)
      {
        csUInt8 *dstPtr = dstPtrY + dx * num;
        const csUInt8 *srcPtr = srcPtrY + sx * num;
        bool right = (sx + 1) < width;
        csUInt8 *nptr = dstPtr;



        for (int c = 0; c < num; ++c, ++srcPtr)
        {
          csInt16 col = convert((csUInt8)*srcPtr, normal);
          int rc = 1;
          if (right)
          {
            col += convert((csUInt8)*(srcPtr + num), normal);
            rc++;
          }
          if (bottom)
          {
            col += convert((csUInt8)*(srcPtr + srcLineScan), normal);
            rc++;

            if (right)
            {
              col += convert((csUInt8)*(srcPtr + srcLineScan + num), normal);
              rc++;
            }
          }
          
          col /= rc;
          col = convertBack(col, normal);
          *dstPtr++ = (csUInt8)(col & 0xff);
        }

        if (normal)
        {
          csUInt8 r = nptr[0];
          csUInt8 g = nptr[1];
          csUInt8 b = nptr[2];
          csVector3f n((float)r / 255.0f * 2.0f - 1.0f, (float)g / 255.0f * 2.0f - 1.0f, (float)b / 255.0f * 2.0f - 1.0f);
          n.Normalize();

          nptr[0] = (csUInt8)((n.x * 0.5f + 0.5f) * 255.0f);
          nptr[1] = (csUInt8)((n.y * 0.5f + 0.5f) * 255.0f);
          nptr[2] = (csUInt8)((n.z * 0.5f + 0.5f) * 255.0f);
        }
      }
    }

    return res;
  }
  
  csUInt8 *generate_mipmap_down_scale(const csUInt8 *data, csUInt16 width, csUInt16 height, csPixelFormat format)
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

bool csImage::GenerateMipMaps(bool normal)
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
  while (true)
  {
    csUInt8 *data = generate_down_scale(m_data[idx++], width, height, m_pixelFormat, normal);
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