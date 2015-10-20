

#include <RenderGL4/TextureGL4.hh>
#include <RenderGL4/MappingGL4.hh>
#include <RenderGL4/DefinesGL4.hh>


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

void vkSamplerGL4::SetMinLOD(vkUInt16 minLOD)
{
  if (m_minLOD != minLOD)
  {
    m_minLOD = minLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MIN_LOD, m_minLOD);
    VK_CHECK_GL_ERROR;
  }
}

vkUInt16 vkSamplerGL4::GetMinLOD() const
{
  return m_minLOD;
}

void vkSamplerGL4::SetMaxLOD(vkUInt16 maxLOD)
{
  if (m_maxLOD != maxLOD)
  {
    m_maxLOD = maxLOD;
    glSamplerParameteri(m_name, GL_TEXTURE_MAX_LOD, m_maxLOD);
    VK_CHECK_GL_ERROR;
  }
}

vkUInt16 vkSamplerGL4::GetMaxLOD() const
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


vkTexture2DGL4::vkTexture2DGL4()
  : vkTextureGL4(eTT_Texture2D)
  , ITexture2D()
{

}

vkTexture2DGL4::~vkTexture2DGL4()
{

}

bool vkTexture2DGL4::Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height)
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
  glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(m_target, 0, internalFormatMap[format], width, height, 0, externalFormatMap[format], externalFormatTypeMap[format], 0);
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

