
#include <GraphicsGL4/gl4texturecube.hh>
#include <GraphicsGL4/gl4mapping.hh>
#include <GraphicsGL4/gl4defines.hh>


vkTextureCubeGL4::vkTextureCubeGL4()
  : vkTextureGL4(eTT_TextureCube)
  , ITextureCube()
{

}

vkTextureCubeGL4::~vkTextureCubeGL4()
{

}

bool vkTextureCubeGL4::Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth)
{
  if (!vkTextureGL4::Initialize())
  {
    return false;
  }

  m_format = format;
  m_width = width;
  m_height = height;
  m_depth = depth;

  Bind();
  glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormatMap[format], depth, height, 0, externalFormatMap[format], externalFormatTypeMap[format], 0);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormatMap[format], depth, height, 0, externalFormatMap[format], externalFormatTypeMap[format], 0);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormatMap[format], width, height, 0, externalFormatMap[format], externalFormatTypeMap[format], 0);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormatMap[format], height, height, 0, externalFormatMap[format], externalFormatTypeMap[format], 0);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormatMap[format], width, depth, 0, externalFormatMap[format], externalFormatTypeMap[format], 0);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormatMap[format], height, depth, 0, externalFormatMap[format], externalFormatTypeMap[format], 0);
  VK_CHECK_GL_ERROR;

  return true;
}

bool vkTextureCubeGL4::CopyData(vkTextureCubeFace face, vkUInt8 lod, vkPixelFormat format, const void *data)
{
  vkUInt16 width;
  vkUInt16 height;
  switch (face)
  {
  case eTCF_PositiveX:
  case eTCF_NegativeX:
    width = m_depth;
    height = m_height;
    break;
  case eTCF_PositiveY:
  case eTCF_NegativeY:
    width = m_width;
    height = m_height;
    break;
  case eTCF_PositiveZ:
  case eTCF_NegativeZ:
    width = m_width;
    height = m_depth;
    break;
  }
  vkUInt16 layerWidth = width >> lod;
  vkUInt16 layerHeight = height >> lod;
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
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, lod, internalFormatMap[m_format], layerWidth, layerHeight, 0, externalFormatMap[format], externalFormatTypeMap[format], data);
  VK_CHECK_GL_ERROR;

  return true;
}


vkUInt16 vkTextureCubeGL4::GetWidth() const
{
  return m_width;
}

vkUInt16 vkTextureCubeGL4::GetHeight() const
{
  return m_height;
}

vkUInt16 vkTextureCubeGL4::GetDepth() const
{
  return m_depth;
}
