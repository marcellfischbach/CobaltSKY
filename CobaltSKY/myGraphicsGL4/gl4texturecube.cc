
#include <graphicsgl4/gl4texturecube.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>


csTextureCubeGL4::csTextureCubeGL4()
  : csTextureGL4(eTT_TextureCube)
  , iTextureCube()
{

}

csTextureCubeGL4::~csTextureCubeGL4()
{

}

bool csTextureCubeGL4::Initialize(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth)
{
  if (!csTextureGL4::Initialize())
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
  CS_CHECK_GL_ERROR;

  return true;
}

bool csTextureCubeGL4::CopyData(csTextureCubeFace face, csUInt8 lod, csPixelFormat format, const void *data)
{
  csUInt16 width;
  csUInt16 height;
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
  csUInt16 layerWidth = width >> lod;
  csUInt16 layerHeight = height >> lod;
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
  CS_CHECK_GL_ERROR;

  return true;
}


csUInt16 csTextureCubeGL4::GetWidth() const
{
  return m_width;
}

csUInt16 csTextureCubeGL4::GetHeight() const
{
  return m_height;
}

csUInt16 csTextureCubeGL4::GetDepth() const
{
  return m_depth;
}
