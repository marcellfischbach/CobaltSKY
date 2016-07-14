

#include <GraphicsGL4/TextureGL4.hh>
#include <GraphicsGL4/MappingGL4.hh>
#include <GraphicsGL4/DefinesGL4.hh>


vkSamplerGL4::vkSamplerGL4()
  : ISampler()
{
  VK_CLASS_GEN_CONSTR;
}

vkSamplerGL4::~vkSamplerGL4()
{

}

bool vkSamplerGL4::Initialize()
{
  glGenSamplers(1, &m_name);
  glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glSamplerParameterf(m_name, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
  glSamplerParameteri(m_name, GL_TEXTURE_MIN_LOD, -1000);
  glSamplerParameteri(m_name, GL_TEXTURE_MAX_LOD, 1000);
  glSamplerParameteri(m_name, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glSamplerParameteri(m_name, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameteri(m_name, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_FUNC, GL_ALWAYS);

  GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  glSamplerParameterfv(m_name, GL_TEXTURE_BORDER_COLOR, color);

  m_filterMode = eFM_MinMagMipNearest;
  m_addressU = eTAM_Repeat;
  m_addressV = eTAM_Repeat;
  m_addressW = eTAM_Repeat;
  m_minLOD = -1000;
  m_maxLOD = -1000;
  m_anisotropy = 1.0f;
  m_borderColor = vkVector4f(0.0f, 0.0f, 0.0f, 0.0f);
  m_textureCompareMode = eTCM_CompareToR;
  m_textureCompareFunc = eTCF_Always;

  VK_CHECK_GL_ERROR;
  return m_name != 0;
}

void vkSamplerGL4::Bind(GLuint unit)
{
  glBindSampler(unit, m_name);
}

void vkSamplerGL4::SetFilter(vkFilterMode filterMode)
{
  if (m_filterMode != filterMode)
  {
    m_filterMode = filterMode;
    switch (filterMode)
    {
    case eFM_MinMagNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinNearestMagLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case eFM_MinLinearMagNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinMagLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    case eFM_MinMagMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinMagNearestMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinNearestMagLinearMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case eFM_MinNearestMagMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    case eFM_MinLinearMagMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinLinearMagNearestMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case eFM_MinMagLinearMipNearest:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case eFM_MinMagMipLinear:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    case eFM_Anisotropic:
      glSamplerParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glSamplerParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;

    }
    VK_CHECK_GL_ERROR;
  }
}

vkFilterMode vkSamplerGL4::GetFilter() const
{
  return m_filterMode;
}

void vkSamplerGL4::SetAnisotropy(vkUInt8 anisotropy)
{
  if (m_anisotropy != anisotropy)
  {
    m_anisotropy = anisotropy;
    glSamplerParameterf(m_name, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropy);
    VK_CHECK_GL_ERROR;
  }
}

vkUInt8 vkSamplerGL4::GetAnisotropy() const
{
  return m_anisotropy;
}

void vkSamplerGL4::SetMinLOD(vkInt16 minLOD)
{
  if (m_minLOD != minLOD)
  {
    m_minLOD = minLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MIN_LOD, m_minLOD);
    VK_CHECK_GL_ERROR;
  }
}

vkInt16 vkSamplerGL4::GetMinLOD() const
{
  return m_minLOD;
}

void vkSamplerGL4::SetMaxLOD(vkInt16 maxLOD)
{
  if (m_maxLOD != maxLOD)
  {
    m_maxLOD = maxLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MAX_LOD, m_maxLOD);
    VK_CHECK_GL_ERROR;
  }
}

vkInt16 vkSamplerGL4::GetMaxLOD() const
{
  return m_maxLOD;
}

void vkSamplerGL4::SetAddressU(vkTextureAddressMode addressMode)
{
  if (m_addressU != addressMode)
  {
    m_addressU = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_S, textureAddressModeMap[m_addressU]);
    VK_CHECK_GL_ERROR;
  }
}

vkTextureAddressMode vkSamplerGL4::GetAddressU() const
{
  return m_addressU;
}

void vkSamplerGL4::SetAddressV(vkTextureAddressMode addressMode)
{
  if (m_addressV != addressMode)
  {
    m_addressV = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_T, textureAddressModeMap[m_addressV]);
    VK_CHECK_GL_ERROR;
  }
}

vkTextureAddressMode vkSamplerGL4::GetAddressV() const
{
  return m_addressV;
}

void vkSamplerGL4::SetAddressW(vkTextureAddressMode addressMode)
{
  if (m_addressW != addressMode)
  {
    m_addressW = addressMode;
    glSamplerParameteri(m_name, GL_TEXTURE_WRAP_R, textureAddressModeMap[m_addressW]);
    VK_CHECK_GL_ERROR;
  }
}

vkTextureAddressMode vkSamplerGL4::GetAddressW() const
{
  return m_addressW;
}


void vkSamplerGL4::SetBorderColor(const vkVector4f &color)
{
  if (m_borderColor != color)
  {
    m_borderColor = color;
    glSamplerParameterfv(m_name, GL_TEXTURE_BORDER_COLOR, static_cast<GLfloat*>(&m_borderColor.x));
    VK_CHECK_GL_ERROR;
  }
}

const vkVector4f &vkSamplerGL4::GetBorderColor() const
{
  return m_borderColor;
}

void vkSamplerGL4::SetTextureCompareMode(vkTextureCompareMode compareMode)
{
  if (m_textureCompareMode != compareMode)
  {
    m_textureCompareMode = compareMode;
    glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_MODE, textureCompareModeMap[compareMode]);
    VK_CHECK_GL_ERROR;
  }
}

vkTextureCompareMode vkSamplerGL4::GetTextureCompareMode() const
{
  return m_textureCompareMode;
}

void vkSamplerGL4::SetTextureCompareFunc(vkTextureCompareFunc compareFunc)
{
  if (m_textureCompareFunc != compareFunc)
  {
    m_textureCompareFunc = compareFunc;
    glSamplerParameteri(m_name, GL_TEXTURE_COMPARE_FUNC, textureCompareFuncMap[compareFunc]);
    VK_CHECK_GL_ERROR;
  }
}

vkTextureCompareFunc vkSamplerGL4::GetTextureCompareFunc() const
{
  return m_textureCompareFunc;
}

bool vkSamplerGL4::NeedsMipMaps() const
{
  return m_filterMode >= eFM_MinMagMipNearest;
}



vkTextureGL4::vkTextureGL4(vkTextureType type)
  : ITexture()
  , m_name(0)
  , m_type(type)
  , m_target(textureTypeMap[type])
  , m_sampler(0)
{
  VK_CLASS_GEN_CONSTR;
}

vkTextureGL4::~vkTextureGL4()
{
  if (m_name)
  {
    glDeleteTextures(1, &m_name);
    VK_CHECK_GL_ERROR;
    m_name = 0;
  }
  VK_RELEASE(m_sampler);
  m_sampler = 0;
}

bool vkTextureGL4::Initialize()
{
  glGenTextures(1, &m_name);
  VK_CHECK_GL_ERROR;
  return m_name != 0;
}

void vkTextureGL4::SetSampler(ISampler *sampler)
{
  VK_SET(m_sampler, sampler);
}

ISampler *vkTextureGL4::GetSampler()
{
  return m_sampler;
}

const ISampler *vkTextureGL4::GetSampler() const
{
  return m_sampler;
}

void vkTextureGL4::Bind()
{
  glBindTexture(m_target, m_name);
  VK_CHECK_GL_ERROR;
}

vkTextureType vkTextureGL4::GetType() const
{
  return m_type;
}

void vkTextureGL4::GenerateMipMaps()
{
  Bind();
  glGenerateMipmap(m_target);
}


vkTexture2DGL4::vkTexture2DGL4()
  : vkTextureGL4(eTT_Texture2D)
  , ITexture2D()
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
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormatMap[format], depth, height,  0, externalFormatMap[format], externalFormatTypeMap[format], 0);
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
