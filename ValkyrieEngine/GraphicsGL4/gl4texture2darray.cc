
#include <GraphicsGL4/gl4texture2darray.hh>
#include <GraphicsGL4/gl4mapping.hh>
#include <GraphicsGL4/gl4defines.hh>


vkTexture2DArrayGL4::vkTexture2DArrayGL4()
  : vkTextureGL4(eTT_Texture2DArray)
  , ITexture2DArray()
{

}

vkTexture2DArrayGL4::~vkTexture2DArrayGL4()
{

}

bool vkTexture2DArrayGL4::Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers, bool mipmaps)
{
  if (!vkTextureGL4::Initialize())
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
  VK_CHECK_GL_ERROR;

  return true;
}

bool vkTexture2DArrayGL4::CopyData(vkUInt8 lod, vkPixelFormat format, const void *data)
{
  vkUInt16 layerWidth = m_width >> lod;
  vkUInt16 layerHeight = m_height >> lod;
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
  VK_CHECK_GL_ERROR;
  glTexImage3D(m_target, lod, internalFormatMap[m_format], layerWidth, layerHeight, m_layers, 0, externalFormatMap[format], externalFormatTypeMap[format], data);
  VK_CHECK_GL_ERROR;

  return true;
}

bool vkTexture2DArrayGL4::CopyData(vkUInt16 layer, vkUInt8 lod, vkPixelFormat format, const void *data)
{
  vkUInt16 layerWidth = m_width >> lod;
  vkUInt16 layerHeight = m_height >> lod;
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
  VK_CHECK_GL_ERROR;
  glTexSubImage3D(m_target, lod, 0, 0, layer, layerWidth, layerHeight, 1, externalFormatMap[format], externalFormatTypeMap[format], data);
  VK_CHECK_GL_ERROR;

  return true;
}


vkUInt16 vkTexture2DArrayGL4::GetWidth() const
{
  return m_width;
}

vkUInt16 vkTexture2DArrayGL4::GetHeight() const
{
  return m_height;
}

vkUInt16 vkTexture2DArrayGL4::GetLayers() const
{
  return m_layers;
}

