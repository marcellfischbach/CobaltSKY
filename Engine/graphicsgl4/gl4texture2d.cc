
#include <graphicsgl4/gl4texture2d.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>

cs::Texture2DGL4::Texture2DGL4()
  : cs::TextureGL4(cs::eTT_Texture2D)
  , cs::iTexture2D()
{

}

cs::Texture2DGL4::~Texture2DGL4()
{
}

bool cs::Texture2DGL4::Initialize(cs::ePixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps)
{
  if (!cs::TextureGL4::Initialize())
  {
    return false;
  }

  m_format = format;
  m_width = width;
  m_height = height;

  Bind();
  glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  CS_CHECK_GL_ERROR;

  int lod = 0;
  while (width >= 1 || height >= 1)
  {
    lod++;
    width >>= 1;
    height >>= 1;
    if (!mipmaps)
    {
      break;
    }
  }
  m_lods = lod;
  glTexStorage2D(m_target, m_lods, sizedInternalFormatMap[format], m_width, m_height);
  CS_CHECK_GL_ERROR;

  return true;
}

bool cs::Texture2DGL4::CopyData(csUInt8 layer, cs::ePixelFormat format, const void *data)
{
  csUInt16 layerWidth = m_width >> layer;
  csUInt16 layerHeight = m_height >> layer;
  if (layerWidth == 0 && layerHeight == 0)
  {
    return false;
  }
  if (layerWidth == 0)
  {
    layerWidth = 1;
  }
  if (layerHeight == 0)
  {
    layerHeight = 1;
  }

  Bind();
  glTexSubImage2D(m_target, layer, 0, 0, layerWidth, layerHeight, externalFormatMap[format], externalFormatTypeMap[format], data);
  CS_CHECK_GL_ERROR;

  return true;
}

bool cs::Texture2DGL4::ReadData(csUInt8 layer, cs::ePixelFormat format, unsigned bufferSize, void *data, unsigned &dataSize)
{
  csUInt16 layerWidth = m_width >> layer;
  csUInt16 layerHeight = m_height >> layer;
  if (layerWidth == 0 && layerHeight == 0)
  {
    return false;
  }
  if (layerWidth == 0)
  {
    layerWidth = 1;
  }
  if (layerHeight == 0)
  {
    layerHeight = 1;
  }

  dataSize = pixelFormatSizeMap[format] * layerWidth * layerHeight;
  if (dataSize > bufferSize || !data)
  {
    return false;
  }

  Bind();
  glGetTexImage(m_target, layer, externalFormatMap[format], externalFormatTypeMap[format], data);
  
  return true;
}


csUInt16 cs::Texture2DGL4::GetWidth() const
{
  return m_width;
}

csUInt16 cs::Texture2DGL4::GetHeight() const
{
  return m_height;
}

csUInt16 cs::Texture2DGL4::GetNumberOfLODs() const
{
  return m_lods;
}


