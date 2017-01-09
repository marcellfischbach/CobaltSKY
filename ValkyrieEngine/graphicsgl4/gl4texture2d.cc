
#include <graphicsgl4/gl4texture2d.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>

vkTexture2DGL4::vkTexture2DGL4()
  : vkTextureGL4(eTT_Texture2D)
  , iTexture2D()
{

}

vkTexture2DGL4::~vkTexture2DGL4()
{
}

bool vkTexture2DGL4::Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height, bool mipmaps)
{
  if (!vkTextureGL4::Initialize())
  {
    return false;
  }

  m_format = format;
  m_width = width;
  m_height = height;

  Bind();
  glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int lod = 0;
  while (width >= 1 || height >= 1)
  {
    glTexImage2D(m_target, lod, internalFormatMap[format], width, height, 0, externalFormatMap[format], externalFormatTypeMap[format], 0);
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

bool vkTexture2DGL4::CopyData(vkUInt8 layer, vkPixelFormat format, const void *data)
{
  vkUInt16 layerWidth = m_width >> layer;
  vkUInt16 layerHeight = m_height >> layer;
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
  glTexImage2D(m_target, layer, internalFormatMap[m_format], layerWidth, layerHeight, 0, externalFormatMap[format], externalFormatTypeMap[format], data);
  VK_CHECK_GL_ERROR;

  return true;
}


vkUInt16 vkTexture2DGL4::GetWidth() const
{
  return m_width;
}

vkUInt16 vkTexture2DGL4::GetHeight() const
{
  return m_height;
}



