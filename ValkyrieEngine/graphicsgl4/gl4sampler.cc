
#include <graphicsgl4/gl4sampler.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>


vkSamplerGL4::vkSamplerGL4()
  : iSampler()
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

