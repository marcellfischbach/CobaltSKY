
#include <graphicsgl4/gl4texture2darray.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>


csTexture2DArrayGL4::csTexture2DArrayGL4()
  : csTextureGL4(eTT_Texture2DArray)
  , iTexture2DArray()
{

}

csTexture2DArrayGL4::~csTexture2DArrayGL4()
{

}

bool csTexture2DArrayGL4::Initialize(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps)
{
  if (!csTextureGL4::Initialize())
  {
    return false;
  }

  m_format = format;
  m_width = width;
  m_height = height;
  m_layers = layers;

  Bind();
  glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  CS_CHECK_GL_ERROR;
  int lod = 0;
  while (width >= 1 || height >= 1)
  {
    glTexImage3D(m_target, lod, internalFormatMap[format], width, height, layers, 0, externalFormatMap[format], externalFormatTypeMap[format], 0);
    lod++;
    width >>= 1;
    height >>= 1;
    if (!mipmaps)
    {
      break;
    }
  }
  CS_CHECK_GL_ERROR;

  return true;
}

bool csTexture2DArrayGL4::CopyData(csUInt8 lod, csPixelFormat format, const void *data)
{
  csUInt16 layerWidth = m_width >> lod;
  csUInt16 layerHeight = m_height >> lod;
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
  glTexImage3D(m_target, lod, internalFormatMap[m_format], layerWidth, layerHeight, m_layers, 0, externalFormatMap[format], externalFormatTypeMap[format], data);
  CS_CHECK_GL_ERROR;

  return true;
}

bool csTexture2DArrayGL4::CopyData(csUInt16 layer, csUInt8 lod, csPixelFormat format, const void *data)
{
  csUInt16 layerWidth = m_width >> lod;
  csUInt16 layerHeight = m_height >> lod;
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
  glTexSubImage3D(m_target, lod, 0, 0, layer, layerWidth, layerHeight, 1, externalFormatMap[format], externalFormatTypeMap[format], data);
  CS_CHECK_GL_ERROR;

  return true;
}


csUInt16 csTexture2DArrayGL4::GetWidth() const
{
  return m_width;
}

csUInt16 csTexture2DArrayGL4::GetHeight() const
{
  return m_height;
}

csUInt16 csTexture2DArrayGL4::GetLayers() const
{
  return m_layers;
}

